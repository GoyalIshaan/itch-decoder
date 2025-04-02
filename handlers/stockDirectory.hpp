#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>
#include "../messages/stockDirectory.hpp"
#include "packedParser.hpp"

namespace handlers {

constexpr size_t STOCK_SYMBOL_SIZE = 8;
constexpr size_t TIMESTAMP_SIZE = 6;

std::string handleStockDirectory(const char* buffer, size_t size, std::ofstream& logFile) {
    // Validate buffer size
    if (size < sizeof(itch::StockDirectoryMessage)) {
        return "Error: Invalid buffer size for StockDirectory message";
    }

    // Parse message using PackedParser
    itch::StockDirectoryMessage msg = itch::PackedParser<itch::StockDirectoryMessage>::parse(buffer, size);

    // Convert fields from network to host byte order
    msg.stock_locate = ntohs(msg.stock_locate);
    msg.tracking_number = ntohs(msg.tracking_number);
    msg.round_lot_size = ntohl(msg.round_lot_size);
    msg.etp_leverage_factor = ntohl(msg.etp_leverage_factor);

    // Generate CSV row
    std::stringstream ss;
    ss << "StockDirectory,";
    
    // Add timestamp (nanoseconds since midnight)
    for (size_t i = 0; i < TIMESTAMP_SIZE; ++i) {
        ss << static_cast<int>(msg.timestamp[i]);
    }
    ss << ",";
    
    // Add stock symbol (trimmed)
    std::string stock_symbol(msg.stock_symbol, STOCK_SYMBOL_SIZE);
    stock_symbol.erase(stock_symbol.find_last_not_of(" ") + 1);
    ss << stock_symbol << ",";
    
    // Add market category and financial status
    ss << msg.market_category << "," << msg.financial_status << ",";
    
    // Add round lot size and round lots only flag
    ss << msg.round_lot_size << "," << msg.round_lots_only << ",";
    
    // Add issue classification and sub type
    ss << msg.issue_classification << "," << std::string(msg.issue_sub_type, 2) << ",";
    
    // Add authenticity and short sale threshold
    ss << msg.authenticity << "," << msg.short_sale_threshold << ",";
    
    // Add IPO flag and LULD tier
    ss << msg.ipo_flag << "," << msg.luld_tier << ",";
    
    // Add ETP flag and leverage factor
    ss << msg.etp_flag << "," << msg.etp_leverage_factor << ",";
    
    // Add inverse indicator
    ss << msg.inverse_indicator;

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