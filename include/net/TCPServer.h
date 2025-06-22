#ifndef INCLUDE_NET_TCPSERVER_H_
#define INCLUDE_NET_TCPSERVER_H_

#define TCPBUFFSIZE 1024

#include <arpa/inet.h>
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

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
