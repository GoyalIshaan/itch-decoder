#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct MWCBDeclineLevelMessage {
    char message_type;       // 'V'
    uint16_t stock_locate;   // Always 0
    uint16_t tracking_number;
    uint8_t timestamp[6];    // Nanoseconds since midnight
    uint64_t level_1_price;  // MWCB Level 1 threshold (Price(8))
    uint64_t level_2_price;  // MWCB Level 2 threshold (Price(8))
    uint64_t level_3_price;  // MWCB Level 3 threshold (Price(8))
};
#pragma pack(pop)

static_assert(sizeof(MWCBDeclineLevelMessage) == 35, "MWCBDeclineLevelMessage struct size is incorrect");

} // namespace itch
