#include"BufferedReader.h"
#include<iostream>

BufferedReader::BufferedReader(const std::string& filename, size_t bufferKB) {
    bufferSize = bufferKB*1024;
    buffer.resize(bufferSize);
    file.open(filename, std::ios::binary);
    if(!file) {
        throw std::runtime_error("Failed to open file");
    }
}

bool BufferedReader::readChunk(std::string& outChunk) {
    if(!file.good()) return false;
    file.read(buffer.data(),bufferSize);
    std::streamsize bytesRead = file.gcount();
    if(bytesRead<=0) return false;
    outChunk.assign(buffer.data(), bytesRead);
    return true;
}

bool BufferedReader:: isOpen() const {
    return file.is_open();
}