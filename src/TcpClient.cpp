#include "TcpClient.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <iostream>

int TcpClient::connectToServer(const std::string& host, int port, int retries) {
    struct sockaddr_in serverAddr{};
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[Error] Socket creation failed.\n";
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "[Error] Invalid address.\n";
        return -1;
    }

    for (int i = 0; i < retries; ++i) {
        if (::connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            std::cout << "[Info] Connected to server.\n";
            return sock;
        }
        std::cerr << "[Warning] Connection attempt " << (i + 1) << " failed. Retrying...\n";
        sleep(1);
    }

    std::cerr << "[Error] Could not connect to server after " << retries << " attempts.\n";
    return -1;
}


void TcpClient::sendAll(int sock, const std::vector<uint8_t>& data) {
    size_t totalSent = 0;
    while (totalSent < data.size()) {
        ssize_t sent = send(sock, data.data() + totalSent, data.size() - totalSent, 0);
        if (sent <= 0) throw std::runtime_error("Send failed");
        totalSent += sent;
    }
}

std::vector<uint8_t> TcpClient::receiveAll(int sock) {
    std::vector<uint8_t> buffer;
    uint8_t temp[1024];
    ssize_t bytesRead;
    while ((bytesRead = recv(sock, temp, sizeof(temp), 0)) > 0) {
        if (bytesRead < 0) {
            std::cerr << "[Error] recv() failed: " << strerror(errno) << "\n";
            break;
        } else if (bytesRead == 0) {
            std::cerr << "[Info] Server closed connection.\n";
            break;
        }

        buffer.insert(buffer.end(), temp, temp + bytesRead);
    }
    return buffer;
}

void TcpClient::closeSocket(int sock) {
    close(sock);
}