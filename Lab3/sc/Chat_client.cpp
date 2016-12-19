#include <pthread.h>
#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "../gen-cpp/Chat.h"
#include "../lock/lock.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace  ::chat;

pthread_mutex_t mutex;
int32_t uid;
int32_t log_id;

void *update(void *arg) {
    ChatClient *client = (ChatClient *) arg;
    std::vector<ChatMessage> missed;

    {
        lock l(&mutex);
        log_id = client->getCurrentLogId();
    }

    while (usleep(50000) == 0) {
        try {
            missed.clear();
            lock l(&mutex);
            client->missedMessages(missed, log_id, uid);
        } catch (...) {
            // TODO: what now???
            return NULL;
        }

        for (ChatMessage &cm : missed) {
            std::cout << cm.username << "] " << cm.text << std::endl;
        }

        log_id += missed.size();
    }
}

int main(int argc, char **argv) {
    boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ChatClient client(protocol);
    bool logged_in = false;
    pthread_t update_thread;
    pthread_mutex_init(&mutex, NULL);

    try {
        transport->open();

        std::string username, message;
        while (!logged_in) {
            username.clear();
            std::cout << "Login: ";
            std::cin >> username;
            try {
                uid = client.get_uid(username);
                std::cout << "Login succesful" << std::endl;
                logged_in = true;
            } catch (...) {
                std::cout << "Try again" << std::endl; //TODO: fix
            }
        }

        pthread_create(&update_thread, NULL, update, (void *) &client);

        ChatMessage msg;
        msg.__set_username(username);
        msg.__set_uid(uid);
        while (true) {
            message.clear();
            std::cin >> message;

            if (message == "!list") {
                std::vector<std::string> user_list;

                {
                    lock l(&mutex);
                    client.getUserList(user_list);
                }

                std::cout << "Connected users: ";
                for (std::string &s : user_list) { std::cout << s << " "; }
                std::cout << std::endl;
            } else {
                msg.__set_text(message);
                lock l(&mutex);
                client.sendMessage(msg);
                log_id++;
            }
        }

        pthread_join(update_thread, NULL);
        transport->close();
    } catch (TException& tx) {
        std::cout << "ERROR: " << tx.what() << std::endl;
    }

    return 0;
}
