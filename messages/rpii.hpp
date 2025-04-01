#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct RetailPriceImprovementIndicatorMessage {
    char message_type;              // 'N'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];           // Nanoseconds since midnight
    char stock[8];                  // Stock symbol, right-padded with spaces
    char interest_flag;             // 'B', 'S', 'A', or 'N'
};
#pragma pack(pop)

static_assert(sizeof(RetailPriceImprovementIndicatorMessage) == 20, "RPII struct size mismatch");

}
