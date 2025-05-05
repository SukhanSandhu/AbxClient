#include <arpa/inet.h> 
#include <sys/socket.h>
#include "AbxClient.h"
#include "TcpClient.h"
#include "JsonBuilder.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>


constexpr int PACKET_SIZE = 17;

AbxClient::AbxClient(const std::string& host, int port) : host(host), port(port) {}

Packet AbxClient::parsePacket(const uint8_t* data) {
    Packet p;
    p.symbol = std::string(reinterpret_cast<const char*>(data), 4);
    p.side = data[4];
    p.quantity = ntohl(*reinterpret_cast<const int32_t*>(data + 5));
    p.price = ntohl(*reinterpret_cast<const int32_t*>(data + 9));
    p.sequence = ntohl(*reinterpret_cast<const int32_t*>(data + 13));
    return p;
}

void AbxClient::streamAllPackets() {
    TcpClient tcp;
    int sock = tcp.connectToServer(host, port);
    tcp.sendAll(sock, {1, 0});
    auto buffer = tcp.receiveAll(sock);
    tcp.closeSocket(sock);

    for (size_t i = 0; i + PACKET_SIZE <= buffer.size(); i += PACKET_SIZE) {
        Packet p = parsePacket(buffer.data() + i);
        sequenceMap[p.sequence] = p;
    }
}

void AbxClient::resendMissingPackets() {
    TcpClient tcp;
    int maxSeq = sequenceMap.rbegin()->first;
    for (int i = 1; i < maxSeq; ++i) {
        if (sequenceMap.count(i)) continue;

        int sock = tcp.connectToServer(host, port);
        tcp.sendAll(sock, {2, static_cast<uint8_t>(i)});

        uint8_t buffer[PACKET_SIZE];
        ssize_t total = recv(sock, buffer, PACKET_SIZE, MSG_WAITALL);
        if (total < 0) {
            std::cerr << "[Error] recv() failed: " << strerror(errno) << "\n";
            break;
        } else if (total == 0) {
            std::cerr << "[Info] Server closed connection.\n";
            break;
        }

        tcp.closeSocket(sock);

        if (total == PACKET_SIZE) {
            Packet p = parsePacket(buffer);
            sequenceMap[p.sequence] = p;
        }
    }
}

void AbxClient::saveToJson(const std::string& filename) {
    std::vector<Packet> packets;
    for (const auto& [seq, pkt] : sequenceMap) packets.push_back(pkt);

    std::string json = JsonBuilder::buildJson(packets);
    std::ofstream out(filename);
    out << json;
    out.close();
}

void AbxClient::run() {
    streamAllPackets();
    resendMissingPackets();
    saveToJson("output/output.json");
}