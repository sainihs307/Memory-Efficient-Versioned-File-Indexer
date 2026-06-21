#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<string>
#include<deque>

class Tokenizer {
private:
    std::deque<std::string> tokens;
    std::string carry;

public: 
    void processChunk(const std::string& chunk);
    bool getNextToken(std::string& token);
    void flush();
};

#endif