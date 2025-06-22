#include "net/TCPServer.h"

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

  if (bind(serverSocket, (struct sockaddr*)&serverAddress,
           sizeof(serverAddress)) == -1) {
    spdlog::error("Error binding socket");
    close(serverSocket);
    return false;
  }

  if (listen(serverSocket, maxConnections) == -1) {
    spdlog::error("Error listening on socket");
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
  clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
                        &clientAddressSize);
  if (clientSocket == -1) {
    spdlog::error("Error accepting connection");
    return -1;
  }

  return clientSocket;
}

std::string TCPServer::readFromClient() {
  char buffer[TCPBUFFSIZE];
  memset(buffer, 0, sizeof(buffer));
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesRead == 0) {
    spdlog::info("Client disconnected");
    return "";
  }
  return std::string(buffer, bytesRead);
}

bool TCPServer::writeToClient(const std::string& data) {
  size_t strLength = data.length();
  size_t totalSent = 0;
  while (totalSent < strLength) {
    ssize_t bytesSent =
        send(clientSocket, data.c_str() + totalSent, strLength - totalSent, 0);
    if (bytesSent == -1) {
      spdlog::error("Error writing to client");
      return false;
    }
    totalSent += bytesSent;
  }
  return true;
}

bool TCPServer::start(uint16_t port) { return initSocket(port); }

void TCPServer::stop() { closeSockets(); }

bool TCPServer::isClientConnected() const { return clientSocket != -1; }
