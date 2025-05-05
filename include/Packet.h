#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <cstdint>

struct Packet {
    std::string symbol; // 4 chars
    char side;           // 'B' or 'S'
    int32_t quantity;
    int32_t price;
    int32_t sequence;
};

#endif // PACKET_H