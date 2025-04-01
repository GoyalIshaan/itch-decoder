#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct BrokenTradeMessage {
    char message_type;             // 'B'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // Nanoseconds since midnight
    uint64_t match_number;         // Match number of the trade being broken
};
#pragma pack(pop)

static_assert(sizeof(BrokenTradeMessage) == 19, "BrokenTradeMessage struct size mismatch");

}
