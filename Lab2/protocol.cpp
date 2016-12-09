#include "protocol.h"
#include <iostream>

unsigned int hashcode(const std::string &s) {
    unsigned int hash = 5381;
    const char *c_str = s.c_str();
    for (int i = 0; c_str[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + c_str[i];
    }

    return hash;
}

bool check(const std::string &message, uint32 len, uint32 checksum) {
    return message.size() == len && hashcode(message) == checksum;
}

void copy(char *dest, const char *src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int good_send(int to_fd, char *send_from, int bytes_n) {
    int retval, bs, bytes_sent = 0, error = 0;
    socklen_t len = sizeof (error);

    retval = getsockopt(to_fd, SOL_SOCKET, SO_ERROR, &error, &len);
    if (retval != 0 || error != 0) {
        return -1; // socket is not alive
    }
    
    while (bytes_sent != bytes_n) {
        bs = send(to_fd, send_from + bytes_sent, bytes_n - bytes_sent, 0);
        if (bs > 0) { bytes_sent += bs; } else { return -1; }
    }

    return bytes_sent;
}

int good_read(int from_fd, char *read_to, int bytes_n) {
    int retval, br, bytes_read = 0, error = 0;
    socklen_t len = sizeof (error);

    retval = getsockopt (from_fd, SOL_SOCKET, SO_ERROR, &error, &len);
    if (retval != 0 || error != 0) {
        return -1; // socket is not alive
    }
    
    while (bytes_read != bytes_n) {
        br = recv(from_fd, read_to + bytes_read, bytes_n - bytes_read, 0);
        if (br > 0) { bytes_read += br; } else { return -1; }
    }

    return bytes_read;
}

int read_msg(int from_fd, std::string &buffer) {
    ssize_t total_bytes_read = 0;
    uint32 len, checksum;

    int br; // bytes read

    br = good_read(from_fd, (char *) &len, sizeof(len));
    if (br > 0) {
        total_bytes_read += br;
        len = be32toh(len);
    } else { return -1; }

    br = good_read(from_fd, (char *) &checksum, sizeof(checksum));
    if (br > 0) {
        total_bytes_read += br;
        checksum = be32toh(checksum);
    } else { return -1; }

    
    char c_str[len];
    br = good_read(from_fd, c_str, len);
    if (br > 0) {
        total_bytes_read += br;
        buffer.append(c_str, len);
    } else { return -1; }
    
    return check(buffer, len, checksum) ? total_bytes_read : -1;
}

int send_msg(int to_fd, std::string &message) {
    ssize_t total_bytes_sent = 0;
    uint32 len = htobe32(message.size()), checksum = htobe32(hashcode(message));
    
    
    int bs; // bytes sent

    bs = good_send(to_fd, (char *) &len, sizeof(len));
    if (bs > 0) { total_bytes_sent += bs; } else { return -1; }

    bs = good_send(to_fd, (char *) &checksum, sizeof(checksum));
    if (bs > 0) { total_bytes_sent += bs; } else { return -1; }

    char c_str[be32toh(len)];
    copy(c_str, message.data(), be32toh(len));
    
    bs = good_send(to_fd, c_str, be32toh(len));
    if (bs > 0) { total_bytes_sent += bs; } else { return -1; }

    return total_bytes_sent;
}
