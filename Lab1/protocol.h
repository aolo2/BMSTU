#ifndef NETWORKS_PROTOCOL_H
#define NETWORKS_PROTOCOL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <string>

// порядок байт - done
// отправка и ПОЛУЧЕНИЕ меньшего колва байт, чем желаем - done
// мьютекс лок и исключение - done
// коды возврата всюду - done
// идентификаторы (хотя бы ip) - done
// инициализация потоков - done
// несколько сообщений подряд - yes!
// выравнивание структур - done (стуктур нет)

#define PORT 22000
#define BUF_SIZE 16
#define DEV_MODE false

typedef unsigned int uint32;

unsigned int hashcode(const std::string &s);

bool check(const std::string &s, uint32 checksum, uint32 len);

int send_msg(int to_fd, std::string &message);

int read_msg(int from_fd, std::string &buffer);

#endif //NETWORKS_PROTOCOL_H
