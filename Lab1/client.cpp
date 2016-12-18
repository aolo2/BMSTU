#include <arpa/inet.h>
#include <iostream>
#include <csignal>


#include "protocol.h"

void *listener(void *arg) {
    int fd = *((int *) arg);
    std::string message;
    
    while (true) {
        message.clear();
        if (read_msg(fd, message) == -1) {
            break;
        } else {
            std::cout << message << std::endl;
        }
    }    
    
    return NULL;
}

int main() {
    int sock_fd;
    sockaddr_in server;
    signal(SIGPIPE, SIG_IGN);

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

    while (true) {
        message.clear();
        std::getline(std::cin, message);
        if (send_msg(sock_fd, message) == -1) {
            break;
        }
    }

    pthread_join(listen_thread, NULL);
    std::cout << "You have (been) disconnected" << std::endl;
    close(sock_fd);

    return 0;
}
