#ifndef MARKET_HPP
#define MARKET_HPP

#include "utils.hpp"

using namespace std;

class Market {
    private:
        void addOrder(Order orderToAdd);
        void modifyOrder(uint64_t orderIdToModify, Order orderToModify);
        void removeOrder(uint64_t orderIdToRemove);
        unordered_map<uint64_t, Order> GetAllOrders();
        unordered_map<string, OrderBook> GetOrderBook(string symbolOfOrderBook);

    public:
        unordered_map<uint64_t, Order> orderTable;
        unordered_map<string, OrderBook> orderBooks;
};



#endif 