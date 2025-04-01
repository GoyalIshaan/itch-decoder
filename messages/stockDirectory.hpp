#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct StockDirectoryMessage {
    char message_type;               // 'R'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];            // Nanoseconds since midnight
    char stock_symbol[8];            // Right-padded with spaces
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
#pragma pack(pop)

static_assert(sizeof(StockDirectoryMessage) == 39, "StockDirectoryMessage struct size is incorrect");

} // namespace itch
