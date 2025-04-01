#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OrderExecutedWithPriceMessage {
    char message_type;             // 'C'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // ID of the order being executed
    uint32_t executed_shares;      // Number of shares executed
    uint64_t match_number;         // Unique match ID for this execution
    char printable;                // 'Y' = show in T&S, 'N' = hidden
    uint32_t execution_price;      // Actual matched price (6.4 fixed format)
};
#pragma pack(pop)

static_assert(sizeof(OrderExecutedWithPriceMessage) == 36, "OrderExecutedWithPriceMessage size mismatch");

}
