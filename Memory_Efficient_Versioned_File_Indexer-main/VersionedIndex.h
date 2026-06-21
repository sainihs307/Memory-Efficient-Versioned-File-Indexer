#ifndef VERSIONED_INDEX_H
#define VERSIONED_INDEX_H

#include<string>
#include<unordered_map>
#include<vector>
#include<cstdint>

class VersionedIndex {
private:
    std::unordered_map<std::string, uint64_t> frequencyMap;

public: 
    void addToken(const std::string& token);
    void addToken(const std::string& token, uint64_t count);
    uint64_t getFrequency(const std::string& token) const;
    std::vector<std::pair<std::string,uint64_t>> getAll() const;
};

#endif