#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OrderDeleteMessage {
    char message_type;             // 'D'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // ID of the order being deleted
};
#pragma pack(pop)

static_assert(sizeof(OrderDeleteMessage) == 19, "OrderDeleteMessage size mismatch");

}
