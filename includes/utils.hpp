#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include <chrono>

using namespace std;

struct Order {
    uint64_t id;
    string symbol;
    double price;
    uint32_t quantity;
    bool is_buy;
    chrono::system_clock::time_point timestamp;
};

struct PriceLevel {
    double price;
    uint64_t totalVolume;
    list<uint16_t> orderIds;
    bool operator<(const PriceLevel& other) const {
        return this -> price < other.price;
    };
};

struct OrderBook {
    string symbol;
    priority_queue<PriceLevel> bidLevels;
    priority_queue<PriceLevel, vector<PriceLevel>, greater<PriceLevel>> askLevels;
};

#endif
