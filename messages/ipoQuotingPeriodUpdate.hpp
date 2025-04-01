#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct IPOQuotingMessage {
    char message_type;             // 'K'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    char stock[8];                 // stock symbol (right-padded with spaces)
    uint32_t release_time;         // seconds since midnight
    char qualifier;                // 'A' or 'C'
    uint32_t ipo_price;            // big-endian encoded price
};
#pragma pack(pop)

static_assert(sizeof(IPOQuotingMessage) == 28, "IPOQuotingMessage struct size is incorrect");

}
