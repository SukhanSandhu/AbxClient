#include "JsonBuilder.h"

std::string JsonBuilder::buildJson(const std::vector<Packet>& packets) {
    std::string json = "[";
    for (size_t i = 0; i < packets.size(); ++i) {
        const Packet& p = packets[i];
        json += "{\"symbol\":\"" + p.symbol + "\",";
        json += "\"side\":\"" + std::string(1, p.side) + "\",";
        json += "\"quantity\":" + std::to_string(p.quantity) + ",";
        json += "\"price\":" + std::to_string(p.price) + ",";
        json += "\"sequence\":" + std::to_string(p.sequence) + "}";
        if (i != packets.size() - 1) json += ",";
    }
    json += "]";
    return json;
}