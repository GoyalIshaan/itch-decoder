#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct SystemEventMessage {
    char message_type;           // 'S'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];        // Nanoseconds since midnight
    char event_code;             // e.g., 'O' = Start of Messages
};
#pragma pack(pop)

static_assert(sizeof(SystemEventMessage) == 12, "SystemEventMessage struct size is incorrect");

} // namespace itch
