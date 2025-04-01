#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct LULDAuctionCollarMessage {
    char message_type;                  // 'J'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];              // nanoseconds since midnight
    char stock[8];                     // symbol, right-padded with spaces
    uint32_t reference_price;          // price in 6.4 fixed format (big-endian)
    uint32_t upper_auction_price;      // upper collar price (big-endian)
    uint32_t lower_auction_price;      // lower collar price (big-endian)
    uint32_t auction_extension;        // number of extensions to reopening auction
};
#pragma pack(pop)

static_assert(sizeof(LULDAuctionCollarMessage) == 35, "LULDAuctionCollarMessage size mismatch");

}
