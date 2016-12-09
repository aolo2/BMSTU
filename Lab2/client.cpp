#include <iostream>
#include <csignal>

#include "protocol.h"

void *listener(void *arg) {
    int fd = *((int *) arg);
    std::string message;
    Answer ans;
    
    while (true) {
        message.clear();
        if (read_msg(fd, message) == -1) {
            break;
        } else {
            ans.Clear();
            ans.ParseFromString(message);
            if (ans.type() == Answer::LIST) {
                std::cout << "{ ";
                for (int i = 0; i < ans.list_size(); i++) {
                    std::cout << ans.list(i) << " ";
                }
                std::cout << "}" << std::endl;
            } else if (ans.type() == Answer::GREETING) {
                std::cout << ans.text();
                fflush(stdout);
            } else {
                std::cout << ans.sender_name() << "] " << ans.text() << std::endl;
            }
        }
    }    
    
    return NULL;
}

int main() {
    int sock_fd;
    sockaddr_in server;
    signal(SIGPIPE, SIG_IGN);
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("");
        return 1;
    }

    server = {AF_INET, htons(PORT), inet_addr("127.0.0.1")};

    if (connect(sock_fd, (sockaddr *) &server, sizeof(server)) < 0) {
        perror("");
        close(sock_fd);
        return 1;
    }

    pthread_t listen_thread;
    if (pthread_create(&listen_thread, NULL, listener, (void *) &sock_fd) != 0) {
        close(sock_fd);
        return 1;
    }

    std::string message;

    Request req;
    while (true) {
        message.clear();
        std::getline(std::cin, message);
        req.Clear();
        if (message == "!dc") { 
            req.set_type(Request::DC); 
        } else if (message == "!list") {
            req.set_type(Request::LIST); 
        } else {  
            req.set_type(Request::TEXT); 
            req.set_text(message); 
        }
        
        message.clear();
        req.SerializeToString(&message);
        
        if (send_msg(sock_fd, message) == -1) {
            break;
        }
    }

    pthread_join(listen_thread, NULL);
    std::cout << "You have (been) disconnected" << std::endl;
    close(sock_fd);
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
