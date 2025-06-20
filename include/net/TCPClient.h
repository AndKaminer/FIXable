#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define TCPBUFFSIZE 1024

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include <spdlog/spdlog.h>


class TCPClient {
public:
    TCPClient(int timeout);
    bool connectClient(const std::string& ip, uint16_t port);
    void disconnectClient();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    int sock;
    int to;
};

#endif
