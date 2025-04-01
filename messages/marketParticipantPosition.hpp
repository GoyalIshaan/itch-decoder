#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct MarketParticipantPositionMessage {
    char message_type;               // 'L'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];            // Nanoseconds since midnight
    char mpid[4];                    // Market participant ID
    char stock[8];                   // Stock symbol (right-padded with spaces)
    char primary_market_maker;       // 'Y' or 'N'
    char market_maker_mode;          // 'N', 'P', 'S', 'R', 'L'
    char participant_state;          // 'A', 'E', 'W', 'S', 'D'
};
#pragma pack(pop)

static_assert(sizeof(MarketParticipantPositionMessage) == 26, "MarketParticipantPositionMessage struct size is incorrect");

} // namespace itch
