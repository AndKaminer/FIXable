#include "net/TCPServer.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

TCPServer::TCPServer(int maxConnections) {
    this->maxConnections = maxConnections;
    serverSocket = -1;
    clientSocket = -1;
}

bool TCPServer::initSocket(uint16_t port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return false;
    }

    if (listen(serverSocket, maxConnections) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        close(serverSocket);
        return false;
    }

    return true;
}

void TCPServer::closeSockets() {
    if (serverSocket != -1) {
        close(serverSocket);
    }
    if (clientSocket != -1) {
        close(clientSocket);
    }
    serverSocket = -1;
    clientSocket = -1;
}

int TCPServer::acceptClient() {
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        return -1;
    }

    return clientSocket;
}

std::string TCPServer::readFromClient() {
    char buffer[TCPBUFFSIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == 0) {
        std::cerr << "Client disconnected" << std::endl;
        return "";
    }
    return std::string(buffer, bytesRead);
}

bool TCPServer::writeToClient(const std::string& data) {
    size_t strLength = data.length();
    int bytesSent = send(clientSocket, data.c_str(), strLength, 0);
    if (bytesSent == -1) {
        std::cerr << "Error writing to client" << std::endl;
        return false;
    }
    return true;
}

bool TCPServer::start(uint16_t port) {
    if (!initSocket(port)) {
        return false;
    }

    return true;
}

void TCPServer::stop() {
    closeSockets();    
}

void TCPServer::run() {
    if (serverSocket == -1) {
        return;
    }

    acceptClient();
    std::cout << "Client connected" << std::endl;
    std::string clientMessage { readFromClient() };
    while (clientMessage != "") {
        if (!writeToClient(clientMessage)) {
            closeSockets();
            return;
        }
        std::cout << clientMessage << std::endl;
        clientMessage = readFromClient();
    }
}

bool TCPServer::isClientConnected() const {
    return clientSocket != -1;
}
