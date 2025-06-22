#ifndef INCLUDE_NET_TCPCLIENT_H_
#define INCLUDE_NET_TCPCLIENT_H_

#define TCPBUFFSIZE 1024

#include <arpa/inet.h>
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <string>

class TCPClient {
 public:
  explicit TCPClient(int timeout);
  bool connectClient(const std::string& ip, uint16_t port);
  void disconnectClient();
  bool sendMessage(const std::string& message);
  std::string receiveMessage();

 private:
  int sock;
  int to;
};

#endif  // INCLUDE_NET_TCPCLIENT_H_
