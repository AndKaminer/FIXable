#ifndef INCLUDE_NET_TCPSERVER_H_
#define INCLUDE_NET_TCPSERVER_H_

#define TCPBUFFSIZE 1024

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <cstring>

class TCPServer {
 public:
    explicit TCPServer(int maxConnections);
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

#endif  // INCLUDE_NET_TCPSERVER_H_
