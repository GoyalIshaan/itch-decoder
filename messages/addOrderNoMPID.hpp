#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct AddOrderNoMPIDMessage {
    char message_type;             // 'A'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // unique order ID
    char buy_sell_indicator;       // 'B' = Buy, 'S' = Sell
    uint32_t shares;               // # of shares
    char stock[8];                 // stock symbol (right-padded with spaces)
    uint32_t price;                // price in fixed-point (6.4) format
};
#pragma pack(pop)

static_assert(sizeof(AddOrderNoMPIDMessage) == 36, "AddOrderNoMPIDMessage size mismatch");

}
