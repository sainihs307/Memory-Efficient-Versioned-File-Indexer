#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include"VersionedIndex.h"
#include<string>
#include<vector>

class QueryProcessor {
public: 
    uint64_t wordQuery(const VersionedIndex& index, const std::string& word);
    std::vector<std::pair<std::string, uint64_t>> topKQuery(const VersionedIndex& index, size_t k);
    int64_t diffQuery(const VersionedIndex& v1, const VersionedIndex& v2, const std::string& word);
};

#endif