#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct MWCBStatusMessage {
    char message_type;       // 'W'
    uint16_t stock_locate;   // Always 0
    uint16_t tracking_number;
    uint8_t timestamp[6];    // Nanoseconds since midnight
    char breached_level;     // '1', '2', or '3'
};
#pragma pack(pop)

static_assert(sizeof(MWCBStatusMessage) == 12, "MWCBStatusMessage struct size is incorrect");

} // namespace itch
