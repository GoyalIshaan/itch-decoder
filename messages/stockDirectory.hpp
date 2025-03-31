#pragma once
#include <cstdint>
#include <iostream>
#include <stdexcept>

using namespace std;

constexpr int STOCK_DIRECTORY_MSG_SIZE = 39;

struct StockDirectoryMessage {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock_symbol[8];
    char market_category;
    char financial_status;
    uint32_t round_lot_size;
    char round_lots_only;
    char issue_classification;
    char issue_sub_type[2];
    char authenticity;
    char short_sale_threshold;
    char ipo_flag;
    char luld_tier;
    char etp_flag;
    uint32_t etp_leverage_factor;
    char inverse_indicator;
};

inline StockDirectoryMessage parseStockDirectoryMessage(const char* buffer, size_t length) {
    if (!buffer || length < STOCK_DIRECTORY_MSG_SIZE) {
        throw std::invalid_argument("Invalid buffer size for Stock Directory message.");
    }

    StockDirectoryMessage msg = {};

    msg.stock_locate     = (static_cast<uint8_t>(buffer[1]) << 8) | static_cast<uint8_t>(buffer[2]);
    msg.tracking_number  = (static_cast<uint8_t>(buffer[3]) << 8) | static_cast<uint8_t>(buffer[4]);

    msg.timestamp =
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[5]))  << 40) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[6]))  << 32) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[7]))  << 24) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[8]))  << 16) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[9]))  << 8)  |
        (static_cast<uint64_t>(static_cast<uint8_t>(buffer[10])));

    msg.stock_symbol[0] = buffer[11];
    msg.stock_symbol[1] = buffer[12];
    msg.stock_symbol[2] = buffer[13];
    msg.stock_symbol[3] = buffer[14];
    msg.stock_symbol[4] = buffer[15];
    msg.stock_symbol[5] = buffer[16];
    msg.stock_symbol[6] = buffer[17];
    msg.stock_symbol[7] = buffer[18];

    msg.market_category       = buffer[19];
    msg.financial_status      = buffer[20];

    msg.round_lot_size =
        (static_cast<uint8_t>(buffer[21]) << 24) |
        (static_cast<uint8_t>(buffer[22]) << 16) |
        (static_cast<uint8_t>(buffer[23]) << 8) |
        static_cast<uint8_t>(buffer[24]);

    msg.round_lots_only       = buffer[25];
    msg.issue_classification  = buffer[26];
    msg.issue_sub_type[0]     = buffer[27];
    msg.issue_sub_type[1]     = buffer[28];
    msg.authenticity          = buffer[29];
    msg.short_sale_threshold  = buffer[30];
    msg.ipo_flag              = buffer[31];
    msg.luld_tier             = buffer[32];
    msg.etp_flag              = buffer[33];

    msg.etp_leverage_factor =
        (static_cast<uint8_t>(buffer[34]) << 24) |
        (static_cast<uint8_t>(buffer[35]) << 16) |
        (static_cast<uint8_t>(buffer[36]) << 8) |
        static_cast<uint8_t>(buffer[37]);

    msg.inverse_indicator     = buffer[38];

    return msg;
}
