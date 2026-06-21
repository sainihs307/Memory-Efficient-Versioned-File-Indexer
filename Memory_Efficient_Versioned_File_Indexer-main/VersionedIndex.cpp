#include"VersionedIndex.h"

void VersionedIndex::addToken(const std::string& token) {
    frequencyMap[token]++;
}

void VersionedIndex::addToken(const std::string& token, uint64_t count) {
    frequencyMap[token] += count;
}

uint64_t VersionedIndex::getFrequency(const std::string& token) const {
    auto it = frequencyMap.find(token);
    if(it==frequencyMap.end()) return 0;
    return it->second;
}

std::vector<std::pair<std::string, uint64_t>> VersionedIndex::getAll() const {
    std::vector<std::pair<std::string, uint64_t>> result;
    for(const auto& pair : frequencyMap) {
        result.push_back(pair);
    }
    return result;
}

