#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OrderExecutedMessage {
    char message_type;             // 'E'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // Order ID
    uint32_t executed_shares;      // Number of shares executed
    uint64_t match_number;         // Unique ID for this execution
};
#pragma pack(pop)

static_assert(sizeof(OrderExecutedMessage) == 31, "OrderExecutedMessage size mismatch");

}
