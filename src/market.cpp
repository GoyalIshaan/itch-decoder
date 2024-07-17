#include "market.hpp"
#include "utils.hpp"

using namespace std;

void Market::addOrder(Order order) {
    uint64_t orderId = order.id;
    orderTable[order.id] = order;
}

void Market::removeOrder(uint64_t orderId) {
}

void Market::modifyOrder(uint64_t orderId, Order order) {
}

unordered_map<uint64_t, Order> Market::GetAllOrders() {
    return orderTable ;
}

unordered_map<string, OrderBook> Market::GetOrderBook(string symbolOfOrderBook) {
    return orderBooks;
}