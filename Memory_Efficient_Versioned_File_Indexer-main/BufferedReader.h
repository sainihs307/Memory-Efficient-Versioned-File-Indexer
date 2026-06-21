#ifndef BUFFERED_READER_H
#define BUFFERED_READER_H

#include"IReader.h"
#include<string.h>
#include<vector>
#include<fstream>

class BufferedReader : public IReader {
private:
    std::ifstream file;
    size_t bufferSize;
    std::vector<char> buffer;

public:
    BufferedReader(const std::string& filename, size_t bufferKB);
    bool readChunk(std::string& outChunk);
    bool isOpen() const;
};

#endif