#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct CrossTradeMessage {
    char message_type;             // 'Q'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // Nanoseconds since midnight
    uint64_t shares;               // Total shares matched in the cross
    char stock[8];                 // Stock symbol, right-padded with spaces
    uint32_t cross_price;          // Match price (6.4 fixed-point)
    uint64_t match_number;         // Unique ID for the cross trade
    char cross_type;               // 'O' (Open), 'C' (Close), 'H' (Reopen)
};
#pragma pack(pop)

static_assert(sizeof(CrossTradeMessage) == 40, "CrossTradeMessage size mismatch");

}
