#ifndef MEMORY_READER_H
#define MEMORY_READER_H

#include"IReader.h"

class MemoryReader : public IReader {
private:
    std::string data;
    size_t position;
    size_t bufferSize;

public: 
    MemoryReader(const std::string& input, size_t bufSize)
    : data(input), position(0), bufferSize(bufSize) {}

    bool readChunk(std::string& outChunk) override {
        if(position >= data.size()) return false;
        size_t remaining = data.size() - position;
        size_t toRead = std::min(bufferSize, remaining);
        outChunk = data.substr(position, toRead);
        position += toRead;
        return true;
    }
};

#endif