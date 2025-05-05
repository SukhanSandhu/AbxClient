# ABX Exchange Client

A C++17 client for the ABX mock exchange server. It connects over TCP, streams binary order book packets, handles missing sequences via re-requests, and outputs a complete JSON file (manually constructed, no JSON libraries used).

## Tested On

- **OS:** Ubuntu 24.04.2 LTS
- **Compiler:** g++ (with C++17 standard)

## Requirements

Make sure you have:
- `g++` (supporting C++17)
- `make`
- POSIX headers (`arpa/inet.h`, `unistd.h`, etc.)

Install essentials on Ubuntu:

sudo apt update

sudo apt install build-essential

Build Instructions

From the root directory:
to clean, run : make clean

to build, run : make

# Run Instructions
form the root folder(or wherever the build is place)
run:
./abxClient

Make sure the ABX exchange server is running on localhost:3000 before running the client.
