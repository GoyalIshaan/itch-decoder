#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct AddOrderAttributionMessage {
    char message_type;             // 'F'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];          // nanoseconds since midnight
    uint64_t order_ref_number;     // unique order ID
    char buy_sell_indicator;       // 'B' or 'S'
    uint32_t shares;               // number of shares
    char stock[8];                 // stock symbol
    uint32_t price;                // price in 6.4 fixed format
    char attribution[4];           // MPID (e.g. 'GSCO', 'NSDQ')
};
#pragma pack(pop)

static_assert(sizeof(AddOrderAttributionMessage) == 40, "AddOrderAttributionMessage size mismatch");

}
