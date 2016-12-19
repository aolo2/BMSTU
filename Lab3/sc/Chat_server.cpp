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
#include "../lock/lock.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::chat;

pthread_mutex_t mutex;

/* 

Login process:

1. Client chooses a username 
2. CLient send his username to server and asks for unique id
3. If username is not taken - client is given a uid
4. If username is taken - cleint is asked to try again (via an exception)



*****
5. Update thread is started by client
6. Every message has the text and the UID


*/

class ChatHandler : virtual public ChatIf {
public:
    ChatHandler() { log_id = 0; }
    
    int32_t getCurrentLogId() { return log_id; }
    
    void getUserList(std::vector<std::string> & _return) {
        lock l(&mutex);
        for (auto &p : username_to_uid) {
            _return.push_back(p.first);
        }
    }
    
    void missedMessages(std::vector<ChatMessage> & _return, const int32_t last, int32_t uid) {
        if (last < log_id) {
            for (int i = last; i < log_id; i++) {
                if (message_log[i].uid != uid) { _return.push_back(message_log[i]); }
            }
        } else if (last > log_id) { // we may have started writing to message_log from 0 again
            for (int i = last; i < message_log.size(); i++) {
               if (message_log[i].uid != uid) { _return.push_back(message_log[i]); }
            }
            
            for (int i = 0; i < log_id; i++) {
                if (message_log[i].uid != uid) { _return.push_back(message_log[i]); }
            }
        }
    }
    
    int32_t get_uid(const std::string& username) {
        if (username_to_uid.count(username)) {
            UsernameTaken ut;
            throw ut;
        }
        
        int32_t uid = generate_uid();
        
        {
            //lock l(&mutex);
            username_to_uid[username] = uid;
            uid_to_username[uid] = username;
        }
        
        std::cout << "Generated random uid for user \'" << username << "\': " << uid << std::endl;
        return uid;
    }

    void sendMessage(const ChatMessage& msg) {
        std::cout << msg.username << "] " << msg.text << std::endl;    
        
        /* Add message to the log. If it reached max size, reset counter. */
        if (log_id > 1000) { log_id = 0; }
        
        lock l(&mutex);
        message_log[log_id++] = msg;
        
        std::cout << log_id << ": " << msg.text << std::endl;
    }

    void disconnectMe() {
        
    }

protected:
    std::map<int32_t, ChatMessage> message_log;
    std::map<std::string, int32_t> username_to_uid;
    std::map<int32_t, std::string> uid_to_username;
    int32_t log_id;
    
    int32_t generate_uid() {
        int32_t ret = rand() % 1000000;
        while (uid_to_username.count(ret) != 0) {
             ret = rand() % 1000000;           
        }
        
        return ret;
    }
};

class ChatCloneFactory : virtual public ChatIfFactory {
public:
    virtual ~ChatCloneFactory() {}
    
    virtual ChatIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        std::cout << "============\nIncoming connection\n============\n";
        std::cout << "    SocketInfo: "  << sock->getSocketInfo() << "\n";
        std::cout << "    PeerHost: "    << sock->getPeerHost() << "\n";
        std::cout << "    PeerAddress: " << sock->getPeerAddress() << "\n";
        std::cout << "    PeerPort: "    << sock->getPeerPort() << "\n";
        std::cout << "============" << "\n";
        
        if (users == 0) {
            ch = new ChatHandler;
        }
    
        users++;
        return ch;
    }
    
    virtual void releaseHandler(ChatIf* handler) {
        if (users == 0) {
            delete handler;
        }
    }
    
private:
    int users = 0;
    ChatHandler *ch;
};


int main(int argc, char **argv) {
    TThreadedServer server(
        boost::make_shared<ChatProcessorFactory>(boost::make_shared<ChatCloneFactory>()),
        boost::make_shared<TServerSocket>(9090), //port
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());
    
    std::cout << "Starting the server..." << std::endl;
    pthread_mutex_init(&mutex, NULL);
    server.serve();
    std::cout << "Done." << std::endl;
    return 0;
}

