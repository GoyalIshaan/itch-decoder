#ifndef MARKET_HPP
#define MARKET_HPP

#include "utils.hpp"

using namespace std;

class Market {
    private:
        void addOrder(const Order& orderToAdd);
        void modifyOrder(uint64_t orderIdToModify, Order newOrder = Order());
        void removeOrder(uint64_t orderIdToRemove);
        const unordered_map<uint64_t, Order> GetAllOrders() const;
        const unordered_map<string, OrderBook> GetOrderBook(string symbolOfOrderBook) const;

    public:
        unordered_map<uint64_t, Order> orderTable;
        unordered_map<string, OrderBook> orderBooks;
};



#endif 