#include"Tokenizer.h"
#include<cctype>

void Tokenizer::processChunk(const std::string& chunk) {
    std::string currentToken = carry;
    carry.clear();
    for(char c : chunk) {
        if(std::isalnum(static_cast<unsigned char>(c))) currentToken += std::tolower(static_cast<unsigned char>(c));
        else {
            if(!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
    }
    if(!currentToken.empty()) {
        carry = currentToken;
    }
}

bool Tokenizer::getNextToken(std::string& token) {
    if(tokens.empty()) return false;
    token = tokens.front();
    tokens.pop_front();
    return true;
}

void Tokenizer::flush() {
    if(!carry.empty()) {
        tokens.push_back(carry);
        carry.clear();
    }
}