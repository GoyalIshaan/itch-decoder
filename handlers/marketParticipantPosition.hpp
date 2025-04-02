#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>
#include "../messages/marketParticipantPosition.hpp"
#include "../utils/packedParser.hpp"
#include "../utils/byteOrder.hpp"

namespace handlers {

constexpr size_t STOCK_SYMBOL_SIZE = 8;
constexpr size_t TIMESTAMP_SIZE = 6;
constexpr size_t MPID_SIZE = 4;

std::string handleMarketParticipantPosition(const char* buffer, size_t size, std::ofstream& logFile) {
    // Validate buffer size
    if (size < sizeof(itch::MarketParticipantPositionMessage)) {
        return "Error: Invalid buffer size for MarketParticipantPosition message";
    }

    // Parse message using PackedParser
    itch::MarketParticipantPositionMessage msg;
    PackedParser parser(buffer, size);
    parser.parse(msg);

    // Convert fields from network to host byte order
    msg.stock_locate = ntohs(msg.stock_locate);
    msg.tracking_number = ntohs(msg.tracking_number);

    // Generate CSV row
    std::stringstream ss;
    ss << "MarketParticipantPosition,";
    
    // Add timestamp (nanoseconds since midnight)
    for (size_t i = 0; i < TIMESTAMP_SIZE; ++i) {
        ss << static_cast<int>(msg.timestamp[i]);
    }
    ss << ",";
    
    // Add MPID (trimmed)
    std::string mpid(msg.mpid, MPID_SIZE);
    mpid.erase(mpid.find_last_not_of(" ") + 1);
    ss << mpid << ",";
    
    // Add stock symbol (trimmed)
    std::string stock_symbol(msg.stock, STOCK_SYMBOL_SIZE);
    stock_symbol.erase(stock_symbol.find_last_not_of(" ") + 1);
    ss << stock_symbol << ",";
    
    // Add market maker flags and participant state
    ss << msg.primary_market_maker << ","
       << msg.market_maker_mode << ","
       << msg.participant_state;

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