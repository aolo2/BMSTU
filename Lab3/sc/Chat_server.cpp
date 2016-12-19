#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "../gen-cpp/Chat.h"
#include "../gen-cpp/chat_constants.h"
#include "../lock/lock.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::chat;

/**
 * mutex_msg - отвечает за логи с сообщениями
 * mutex_login - отвечает за книги с именами пользователей
 */

pthread_mutex_t mutex_msg, mutex_login;

class ChatHandler : virtual public ChatIf {
public:
    ChatHandler() { log_id = 0; }

    int32_t getCurrentLogId() { return log_id; }

    /**
     * Реализовано тривиально.
     */
    void getUserList(std::vector<std::string> & _return) {
        lock l(&mutex_login);
        for (auto &p : username_to_uid) {
            _return.push_back(p.first);
        }
    }

    /**
     * Пользователь предоставляет номер последнего "известного" ему сообщения
     * и просит вернуть ему все, которые он еще "не видел". Возможен случай,
     * когда счетчик log_id обнулился, поэтому возможно last > log_id. В таком
     * случае клиенту вовращаются все (не отправленные им самим) сообщения до
     * MAX_HISTORY_SIZE, а потом от 0 до log_id
     */
    void missedMessages(std::vector<ChatMessage> & _return, const int32_t last, int32_t uid) {
        if (last + 1 == log_id || !uid_to_username.count(uid)) { return; }

        if (last + 1 < log_id) {
            for (int i = last + 1; i < log_id; i++) { add_message(_return, uid, i); }
        } else {
            for (int i = last + 1; i < MAX_HISTORY_SIZE; i++) { add_message(_return, uid, i); }
            for (int i = 0; i < log_id; i++) { add_message(_return, uid, i); }
        }
    }

    int32_t get_uid(const std::string& username) {
        /**
         * Пользователь запрашивает уникальный id, предоставляя свой ник.
         * Если такой ник еще не записан в книге, то вызывается метод
         * genereate_uid(), генерирующий уникальный id. Если такой ник уже
         * есть в книге, то кидается исключение UsernameTaken. Клиент сам решает,
         * что делать с этим исключением.
         */
        if (username_to_uid.count(username)) {
            UsernameTaken ut;
            ut.__set_message("Username taken, try again");
            throw ut;
        }

        int32_t uid = generate_uid();

        {
            lock l(&mutex_login);
            username_to_uid[username] = uid;
            uid_to_username[uid] = username;
        }

        std::cout << "Generated random uid for user \'" << username << "\': " << uid << std::endl;
        return uid;
    }

    void sendMessage(const ChatMessage& msg) {
        if (!uid_to_username.count(msg.uid)) { return; }
        /**
         * Каждое принятое сообщение увеличивает счетчик log_id на единицу.
         * Если счетчик достиг MAX_HISTORY_SIZE, то он сбрасивается на ноль,
         * и сообщения будут писаться в лог с нулевого индекса.
         */
        lock l(&mutex_msg);
        message_log[log_id++] = msg;
        std::cout << "[LOG] " << log_id - 1 << ": " << message_log[log_id - 1] << std::endl;
        if (log_id == MAX_HISTORY_SIZE) { log_id = 0; }
    }

    /**
     * Пользователь просит отсоединить его. На сервере самого отсоединения не
     * происходит. Вместо этого записи с его ником и id удаляются из книг,
     * поэтому сообщения как ОТ, так и К нему идти не будут. Однако ответственность
     * за закрытие сокета лежит на самом клиенте.
     */
    void disconnectMe(int32_t uid) {
        if (!uid_to_username.count(uid)) { return; }

        lock l(&mutex_login);
        std::string username_to_remove = uid_to_username[uid];
        uid_to_username.erase(uid);
        username_to_uid.erase(username_to_remove);
    }

protected:
    std::map<int32_t, ChatMessage> message_log;         // лог сообщений
    std::map<std::string, int32_t> username_to_uid;     // ник -> uid
    std::map<int32_t, std::string> uid_to_username;     // uid -> ник
    const int32_t MAX_HISTORY_SIZE = 10;               // макс. длина лога
    const int32_t MAX_USER_COUNT = 1000;                // макс. кол-во клиентов
    int32_t log_id;                                     // номер следующего сообщения

    void add_message(std::vector<ChatMessage> & _return, int32_t uid, int32_t i) {
        lock l(&mutex_msg);
        if (message_log[i].uid != uid) { _return.push_back(message_log[i]); }
    }

    int32_t generate_uid() {
        int32_t ret = rand() % MAX_USER_COUNT;
        lock l(&mutex_login);
        while (uid_to_username.count(ret) != 0) {
             ret = rand() % MAX_USER_COUNT;
        }

        return ret;
    }
};

class ChatCloneFactory : virtual public ChatIfFactory {
public:
    virtual ~ChatCloneFactory() {}

    /**
     * Вовзращаем всем один и тот же ChatHandler, потому что
     * общаться с самим с собой в чате неинтересно (ИМХО).
     */
    virtual ChatIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
        std::cout << "Incoming connection" << std::endl;
        return HANDLER_INSTANCE;
    }

    virtual void releaseHandler(ChatIf* handler) {}

private:
    ChatHandler* HANDLER_INSTANCE = new ChatHandler();
};


int main(int argc, char **argv) {
    TThreadedServer server(
        boost::make_shared<ChatProcessorFactory>(boost::make_shared<ChatCloneFactory>()),
        boost::make_shared<TServerSocket>(9090), //port
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());

    std::cout << "Starting the server..." << std::endl;
    pthread_mutex_init(&mutex_msg, NULL);
    pthread_mutex_init(&mutex_login, NULL);
    server.serve();
    std::cout << "Done." << std::endl;
    return 0;
}
