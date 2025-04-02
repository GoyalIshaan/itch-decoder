#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>
#include "../messages/addOrderWithMPID.hpp"
#include "packedParser.hpp"

namespace handlers {

constexpr size_t STOCK_SYMBOL_SIZE = 8;
constexpr size_t TIMESTAMP_SIZE = 6;
constexpr size_t MPID_SIZE = 4;

std::string handleAddOrderWithMPID(const char* buffer, size_t size, std::ofstream& logFile) {
    // Validate buffer size
    if (size < sizeof(itch::AddOrderAttributionMessage)) {
        return "Error: Invalid buffer size for AddOrderWithMPID message";
    }

    // Parse message using PackedParser
    itch::AddOrderAttributionMessage msg = itch::PackedParser<itch::AddOrderAttributionMessage>::parse(buffer, size);

    // Convert fields from network to host byte order
    msg.stock_locate = ntohs(msg.stock_locate);
    msg.tracking_number = ntohs(msg.tracking_number);
    msg.order_ref_number = ntohll(msg.order_ref_number);
    msg.shares = ntohl(msg.shares);
    msg.price = ntohl(msg.price);

    // Generate CSV row
    std::stringstream ss;
    ss << "AddOrderWithMPID,";
    
    // Add timestamp (nanoseconds since midnight)
    for (size_t i = 0; i < TIMESTAMP_SIZE; ++i) {
        ss << static_cast<int>(msg.timestamp[i]);
    }
    ss << ",";
    
    // Add order reference number
    ss << msg.order_ref_number << ",";
    
    // Add buy/sell indicator
    ss << msg.buy_sell_indicator << ",";
    
    // Add shares
    ss << msg.shares << ",";
    
    // Add stock symbol (trimmed)
    std::string stock_symbol(msg.stock, STOCK_SYMBOL_SIZE);
    stock_symbol.erase(stock_symbol.find_last_not_of(" ") + 1);
    ss << stock_symbol << ",";
    
    // Add price (fixed point 6.4 format)
    double price = static_cast<double>(msg.price) / 10000.0;
    ss << std::fixed << std::setprecision(4) << price << ",";
    
    // Add MPID (trimmed)
    std::string mpid(msg.attribution, MPID_SIZE);
    mpid.erase(mpid.find_last_not_of(" ") + 1);
    ss << mpid;

    std::string csv_row = ss.str();

    // Write to log file
    if (!logFile.is_open()) {
        return "Error: Log file is not open";
    }
    logFile << csv_row << std::endl;
    if (logFile.fail()) {
        return "Error: Failed to write to log file";
    }

    return csv_row;
}

} // namespace handlers 