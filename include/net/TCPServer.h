#ifndef TCPSERVER_H
#define TCPSERVER_H

#define TCPBUFFSIZE 1024

#include <cstdint>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <spdlog/spdlog.h>


class TCPServer {
public:
    TCPServer(int maxConnections);
    bool start(uint16_t port);
    void stop();
    int acceptClient();
    std::string readFromClient();
    bool writeToClient(const std::string& data);
    bool isClientConnected() const;

private:
    int serverSocket;
    int clientSocket;
    int maxConnections;

    bool initSocket(uint16_t port);
    void closeSockets();
};

#endif
