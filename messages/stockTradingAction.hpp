#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct StockTradingActionMessage {
    char message_type;           // 'H'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];        // Nanoseconds since midnight
    char stock_symbol[8];        // Right-padded with spaces
    char trading_state;          // 'H' (halted), 'P' (paused), 'T' (trading)
    char reserved;               // Reserved (usually a space)
    char reason[4];              // Halt reason code
};
#pragma pack(pop)

static_assert(sizeof(StockTradingActionMessage) == 25, "StockTradingActionMessage struct size is incorrect");

} // namespace itch
