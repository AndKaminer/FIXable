#ifndef TCP_H
#define TCP_H

#define TCPBUFFSIZE 1024

#include <cstdint>
#include <string>

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
