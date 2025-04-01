#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OrderReplaceMessage {
    char message_type;               // 'U'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];            // Nanoseconds since midnight
    uint64_t original_order_ref;     // Old order ID being replaced
    uint64_t new_order_ref;          // New order ID to be used henceforth
    uint32_t shares;                 // New total displayed quantity
    uint32_t price;                  // New display price (6.4 fixed format)
};
#pragma pack(pop)

static_assert(sizeof(OrderReplaceMessage) == 35, "OrderReplaceMessage size mismatch");

}
