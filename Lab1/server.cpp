#include <iostream>
#include <map>

#include "protocol.h"
#include "mutex_wrapper.h"

std::map<int, std::string> book;
pthread_mutex_t mutex;


void disconnect(int fd) {
    mutex_wrapper m(&mutex);
    book.erase(fd); // what if exception
    close(fd);
}

void broadcast(int from_fd, std::string &message) {
    std::string to_send = "[";
    to_send += book[from_fd];
    to_send += "] ";
    to_send += message;
    
    for (auto &user : book) {
        if (user.first != from_fd && send_msg(user.first, to_send) == -1) {
            disconnect(user.first);
        }
    }
}

void send_user_list(int to_fd) {
    std::string user_list = "Connected users: {";
    int retval;
    
    {
        mutex_wrapper m(&mutex);
        for (auto &user : book) {
            user_list.append(" ");
            user_list.append(user.second);
        }
        user_list.append(" }");
        retval = send_msg(to_fd, user_list);
    }
    
    if (retval == -1) { disconnect(to_fd); }
}

void parse(int fd, std::string &user_msg) {
    if (user_msg == "!list") {
        send_user_list(fd);
    } else if (user_msg == "!dc") {
        disconnect(fd);
    } else {
        broadcast(fd, user_msg);
    }
}

int login(int fd) {
    std::string greeting = "Greetings! What is your name?";
    if (send_msg(fd, greeting) == -1) {
        return -1;
    } else {
        std::string username;
        if (read_msg(fd, username) == -1) {
            return -1;
        } else {
            mutex_wrapper m(&mutex);
            book[fd] = username;
        }
    }
    
    return 0;
}

void *session(void *arg) {
    int fd = *((int *) arg);

    if (login(fd) == -1) {
        std::cout << "Error occured during login, disconnecting user" << std::endl;
        disconnect(fd);
        return NULL;
    }


    std::string user_msg;
    while (true) {
        user_msg.clear();
        if (read_msg(fd, user_msg) == -1) {
            disconnect(fd);
            break;
        } else {
            parse(fd, user_msg);
        }
    }

    
    std::cout << "User disconnected" << std::endl;
    return NULL;
}

int main() {
    int client_fd, sock_fd, o = 1;
    sockaddr_in self;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
        std::cout << "Created socket, fd = " << sock_fd << std::endl;
    } else {
        perror("");
        return 1;
    }

    self = {AF_INET, htons(PORT), INADDR_ANY};
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o)) == 0) {
        std::cout << "Successfully set socket option" << std::endl;
    } else {
        close(sock_fd);
        perror("");
        return 1;
    };

    if (bind(sock_fd, (sockaddr *) &self, sizeof(self)) == 0) {
        std::cout << "Successfully bound" << std::endl;
    } else {
        close(sock_fd);
        perror("");
        return 1;
    }

    if (listen(sock_fd, 20) == 0) {
        std::cout << "Started listening at port " << PORT << std::endl;
    } else {
        close(sock_fd);
        perror("");
        return 1;
    }

    unsigned int addrlen;
    sockaddr_in client_addr;
    pthread_mutex_init(&mutex, NULL);

    while (true) { // check for connection every second
        addrlen = sizeof(client_addr);
        client_fd = accept(sock_fd, (sockaddr *) &client_addr, &addrlen);

        if (client_fd > 0) {
            pthread_t thread;
            if (pthread_create(&thread, NULL, session, (void *) &client_fd) != 0) {
                book.erase(client_fd); // what if comparator exception
                close(client_fd);
                continue;
            }
        }
    }
}
