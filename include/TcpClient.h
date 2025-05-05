#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <vector>
#include <string>
#include <cstdint>

class TcpClient {
public:
    int connectToServer(const std::string& host, int port, int retries = 5);
    void sendAll(int sock, const std::vector<uint8_t>& data);
    std::vector<uint8_t> receiveAll(int sock);
    void closeSocket(int sock);
};

#endif // TCPCLIENT_H