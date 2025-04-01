#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct TradeMessage {
    char message_type;             // 'P'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // Nanoseconds since midnight
    uint64_t order_ref_number;     // Always 0 for hidden orders
    char buy_sell_indicator;       // Always 'B' since 2014
    uint32_t shares;               // Shares traded
    char stock[8];                 // Stock symbol, right-padded
    uint32_t price;                // Execution price (6.4 fixed-point)
    uint64_t match_number;         // Unique match ID
};
#pragma pack(pop)

static_assert(sizeof(TradeMessage) == 44, "TradeMessage size mismatch");

}
