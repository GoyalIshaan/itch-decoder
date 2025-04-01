#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct RegSHOMessage {
    char message_type;           // 'Y'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];        // Nanoseconds since midnight
    char stock_symbol[8];        // Right-padded with spaces
    char reg_sho_action;         // '0', '1', or '2'
};
#pragma pack(pop)

static_assert(sizeof(RegSHOMessage) == 20, "RegSHOMessage struct size is incorrect");

} // namespace itch
