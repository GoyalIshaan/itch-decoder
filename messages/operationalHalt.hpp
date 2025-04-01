#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct OperationalHaltMessage {
    char message_type;             // 'h'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];         // nanoseconds since midnight
    char stock[8];                // stock symbol, right-padded with spaces
    char market_code;            // 'Q' = Nasdaq, 'B' = BX, 'X' = PSX
    char halt_action;            // 'H' = halted, 'T' = resumed
};
#pragma pack(pop)

static_assert(sizeof(OperationalHaltMessage) == 21, "OperationalHaltMessage size mismatch");

}
