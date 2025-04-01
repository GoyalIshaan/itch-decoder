#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OrderCancelMessage {
    char message_type;             // 'X'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // ID of the order being reduced
    uint32_t cancelled_shares;     // Number of shares being removed
};
#pragma pack(pop)

static_assert(sizeof(OrderCancelMessage) == 23, "OrderCancelMessage size mismatch");

}
