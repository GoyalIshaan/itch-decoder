#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct NOIIMessage {
    char message_type;              // 'I'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];           // Nanoseconds since midnight
    uint64_t paired_shares;         // Shares matched at current ref price
    uint64_t imbalance_shares;      // Shares not matched
    char imbalance_direction;       // 'B', 'S', 'N', 'O', or 'P'
    char stock[8];                  // Right-padded stock symbol
    uint32_t far_price;             // Auction-only orders price (6.4 fixed)
    uint32_t near_price;            // Auction + continuous price (6.4 fixed)
    uint32_t current_reference_price; // Price used for calculations
    char cross_type;                // 'O', 'C', 'H', 'A'
    char price_variation_indicator; // 'L', '1'-'9', 'A'-'C', or space
};
#pragma pack(pop)

static_assert(sizeof(NOIIMessage) == 50, "NOIIMessage struct size mismatch");

}
