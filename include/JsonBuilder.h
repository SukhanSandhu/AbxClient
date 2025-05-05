#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include "Packet.h"
#include <vector>
#include <string>

class JsonBuilder {
public:
    static std::string buildJson(const std::vector<Packet>& packets);
};

#endif // JSONBUILDER_H