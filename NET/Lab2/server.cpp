#include <iostream>
#include <map>

#include "mutex_wrapper.h"
#include "protocol.h"

std::map<int, std::string> book;
std::string bad_login = "Please don't start your nick with \'!\'";
pthread_mutex_t mutex;


void disconnect(int fd) {
    mutex_wrapper m(&mutex);
    book.erase(fd); // what if exception
    close(fd);
}

void broadcast(int from_fd, const std::string &message) {
    Answer ans;
    ans.set_type(Answer::TEXT);
    ans.set_sender_name(book[from_fd]);
    ans.set_text(message);
    
    std::string answer;
    ans.SerializeToString(&answer);
    
    mutex_wrapper m(&mutex);
    for (auto &user : book) {
        if (user.first != from_fd) {
            if (send_msg(user.first, answer) == -1) { 
                disconnect(user.first); 
            }
        }
    }
}

void send_user_list(int to_fd) {
    
    Answer ans;
    ans.set_type(Answer::LIST);
    
    {
        mutex_wrapper m(&mutex);
        for (auto &user : book) {
            ans.add_list(user.second);            
        }
    }
    
    std::string message;
    ans.SerializeToString(&message);
    if (send_msg(to_fd, message) == -1) { disconnect(to_fd); }
}

void parse(int fd, std::string &user_msg) {
    Request req;
    req.ParseFromString(user_msg);
    if (req.type() == Request::DC) {
        disconnect(fd);
    } else if (req.type() == Request::LIST) {
        send_user_list(fd);
    } else {
        broadcast(fd, req.text());
    }
}

int send_server_message(int fd, std::string &s) {
    Answer ans;
    ans.set_type(Answer::TEXT);
    ans.set_sender_name("SERVER");
    ans.set_text(s);
    
    std::string msg;
    ans.SerializeToString(&msg);
    
    if (send_msg(fd, msg) == -1) { return -1; }
    
    return 0;
}

int login(int fd) {
    Answer ans;
    ans.set_type(Answer::GREETING);
    ans.set_text("Greetings! What is your name: ");
    
    std::string greeting;
    ans.SerializeToString(&greeting);
        
    if (send_msg(fd, greeting) == -1) {
        return -1;
    } else {
        std::string username;
        if (read_msg(fd, username) == -1) {
            return -1;
        } else {
            Request req;
            req.ParseFromString(username);
            
            if (req.type() != Request::TEXT) {
                if (send_server_message(fd, bad_login) == -1) {
                    return -1;
                } else {
                    return login(fd);
                }
            }
            
            std::string login_succ;
            login_succ += "Welcome, " + req.text() + " to the channel!\n";
            login_succ += "Usage:\n!dc - disconnect\n!list - get user list";
            
            if (send_server_message(fd, login_succ) == -1) {
                return -1;    
            }
            
            mutex_wrapper m(&mutex);
            book[fd] = req.text();
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
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
        std::cout << "Successfully created socket, fd = " << sock_fd << std::endl;
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
        std::cout << "Successfully started listening at port " << PORT << std::endl;
    } else {
        close(sock_fd);
        perror("");
        return 1;
    }

    unsigned int addrlen;
    sockaddr_in client_addr;
    pthread_mutex_init(&mutex, NULL);

    while (true) {
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
    
    close(sock_fd);
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
