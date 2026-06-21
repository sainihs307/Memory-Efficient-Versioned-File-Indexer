#ifndef IREADER_H
#define IREADER_H

#include<string>

class IReader {
public: 
    virtual bool readChunk(std::string& outchunk) = 0;
    virtual ~IReader() = default;
};

#endif