#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<vector>

template<typename k, typename v> 
void printpairs(const std::vector<std::pair<k,v>>& data) {
    for(const auto& p : data) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}

#endif