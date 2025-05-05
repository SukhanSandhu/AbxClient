#ifndef ABXCLIENT_H
#define ABXCLIENT_H

#include "Packet.h"
#include <map>
#include <vector>
#include <string>

class AbxClient {
public:
    AbxClient(const std::string& host, int port);
    void run();

private:
    std::string host;
    int port;
    std::map<int, Packet> sequenceMap;

    void streamAllPackets();
    void resendMissingPackets();
    void saveToJson(const std::string& filename);
    Packet parsePacket(const uint8_t* data);
};

#endif // ABXCLIENT_H