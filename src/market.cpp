#include "market.hpp"
#include "utils.hpp"

using namespace std;

void Market::addOrder(const Order& order) {
    uint64_t orderId = order.id;
    orderTable[orderId] = order;

    string symbol = order.symbol;

    if (orderBooks.find(symbol) == orderBooks.end()) {
        OrderBook orderBook;
        orderBook.symbol = symbol;
        orderBooks[symbol] = orderBook;
    }

    OrderBook& orderBook = orderBooks[symbol];

    PriceLevel newLevel;
    newLevel.price = order.price;
    newLevel.totalVolume = order.quantity;
    newLevel.orderIds.push_back(orderId);

    if (order.is_buy) {
        bool updated = false;
        priority_queue<PriceLevel> tempBidLevels;

        while (!orderBook.bidLevels.empty())
        {
            PriceLevel currentLevel = orderBook.bidLevels.top();
            orderBook.bidLevels.pop();

            if (currentLevel.price == newLevel.price) {
                updated = true;
                currentLevel.totalVolume += newLevel.totalVolume;
                currentLevel.orderIds.push_back(orderId);
            }

            tempBidLevels.push(currentLevel);
        }

        if (!updated) {
            tempBidLevels.push(newLevel);
        }

        swap(orderBook.bidLevels, tempBidLevels);
    } else {
        bool updated = false;
        priority_queue<PriceLevel, vector<PriceLevel>, greater<PriceLevel>> tempAskLevels;

        while (!orderBook.askLevels.empty()) {
            PriceLevel currentLevel = orderBook.askLevels.top();
            orderBook.askLevels.pop();

            if (currentLevel.price == newLevel.price) {
                updated = true;
                currentLevel.totalVolume += newLevel.totalVolume;
                currentLevel.orderIds.push_back(orderId);
            }

            tempAskLevels.push(currentLevel);
        }

        if (!updated) {
            tempAskLevels.push(newLevel);
        }

        swap(orderBook.askLevels, tempAskLevels);
    }

}

void Market::removeOrder(uint64_t orderId) {
    Order order = orderTable[orderId];
    string symbol = order.symbol;
    OrderBook& orderBook = orderBooks[symbol];

    if (order.is_buy) {
        priority_queue<PriceLevel> tempBidLevels;

        while (!orderBook.bidLevels.empty()) {
            PriceLevel currentLevel = orderBook.bidLevels.top();
            orderBook.bidLevels.pop();

            if (currentLevel.price == order.price) {
                currentLevel.totalVolume -= order.quantity;
                currentLevel.orderIds.remove(orderId);
            }

            if (currentLevel.totalVolume > 0) {
                tempBidLevels.push(currentLevel);
            }
        }

        swap(orderBook.bidLevels, tempBidLevels);
    } else {
        priority_queue<PriceLevel, vector<PriceLevel>, greater<PriceLevel>> tempAskLevels;

        while (!orderBook.askLevels.empty()) {
            PriceLevel currentLevel = orderBook.askLevels.top();
            orderBook.askLevels.pop();

            if (currentLevel.price == order.price) {
                currentLevel.totalVolume -= order.quantity;
                currentLevel.orderIds.remove(orderId);
            }

            if (currentLevel.totalVolume > 0) {
                tempAskLevels.push(currentLevel);
            }
        }

        swap(orderBook.askLevels, tempAskLevels);
    }

    orderTable.erase(orderId);
}

void Market::modifyOrder(uint64_t orderId, Order newOrder) {
    removeOrder(orderId);
    addOrder(newOrder);
}

unordered_map<uint64_t, Order> Market::GetAllOrders() {
    return orderTable ;
}

unordered_map<string, OrderBook> Market::GetOrderBook(string symbolOfOrderBook) {
    return orderBooks;
}