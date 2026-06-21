#include"QueryProcessor.h"
#include<algorithm>

uint64_t QueryProcessor::wordQuery(const VersionedIndex& index, const std::string& word) {
    std::string normalized = word;
    for (char& c : normalized) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return index.getFrequency(normalized);
}

int64_t QueryProcessor::diffQuery(const VersionedIndex& v1, const VersionedIndex& v2, const std::string& word) {
    std::string normalized = word;
    for (char& c : normalized) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    uint64_t f1 = v1.getFrequency(normalized);
    uint64_t f2 = v2.getFrequency(normalized);
    return static_cast<int64_t>(f2) - static_cast<int64_t>(f1);
}

std::vector<std::pair<std::string, uint64_t>> QueryProcessor::topKQuery(const VersionedIndex& index, size_t k) {
    auto all = index.getAll();
    std::sort(all.begin(),all.end(),
    [](const auto& a, const auto& b) {
        if(a.second==b.second) return a.first < b.first;
        return a.second > b.second;
    });
    if(k<all.size()) {
        all.resize(k);
    }
    return all;
}
