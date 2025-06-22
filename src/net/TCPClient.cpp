#include "net/TCPClient.h"


TCPClient::TCPClient(int timeout) :
    sock(-1), to(timeout) {}

bool TCPClient::connectClient(const std::string& ip, uint16_t port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return false;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) != 1) {
        spdlog::error("Invalid address");
        close(sock);
        return false;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct timeval timeout;
    timeout.tv_sec = to;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    if (connect(sock, (struct sockaddr*)&serverAddress,
                (socklen_t)sizeof(serverAddress)) == -1) {
        close(sock);
        spdlog::error("Error connecting to server");
        return false;
    } else {
        spdlog::info("Connected to server");
    }

    return true;
}

void TCPClient::disconnectClient() {
    if (sock != -1) {
        close(sock);
        sock = -1;
    }

    spdlog::info("Disconnected from server");
}

bool TCPClient::sendMessage(const std::string& message) {
    if (sock == -1) {
        spdlog::error("Attempted to send on closed socket");
        return false;
    }
    size_t strLength = message.length();
    size_t totalSent = 0;
    while (totalSent < strLength) {
        ssize_t bytesSent = send(sock, message.c_str() + totalSent,
                strLength - totalSent, 0);
        if (bytesSent == -1) {
            spdlog::error("Error sending message to server: {}",
                    strerror(errno));
            return false;
        }
        totalSent += bytesSent;
    }
    return true;
}

std::string TCPClient::receiveMessage() {
    if (sock == -1) {
        spdlog::error("Attempted to receive on closed socket");
        return "";
    }

    char buffer[TCPBUFFSIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(sock, buffer, sizeof(buffer), 0);
    if (bytesRead == 0) {
        spdlog::info("Server closed connection");
        return "";
    } else if (bytesRead == -1) {
        spdlog::error("Error reading from server: {}", strerror(errno));
        return "";
    }
    return std::string(buffer, bytesRead);
}
