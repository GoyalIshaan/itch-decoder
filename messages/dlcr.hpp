#pragma once
#include <cstdint>

namespace itch {

#pragma pack(push, 1)
struct DirectListingWithCapitalRaiseMessage {
    char message_type;                // 'O'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint8_t timestamp[6];            // Nanoseconds since midnight
    char stock[8];                   // Right-padded stock symbol
    char open_eligibility_status;    // 'Y' or 'N'
    uint32_t min_allowable_price;    // 20% below registration range floor
    uint32_t max_allowable_price;    // 80% above registration range ceiling
    uint32_t near_execution_price;   // Candidate auction clearing price
    uint64_t near_execution_time;    // Time near execution price was set (ns since midnight)
    uint32_t lower_price_collar;     // 10% below near execution price
    uint32_t upper_price_collar;     // 10% above near execution price
};
#pragma pack(pop)

static_assert(sizeof(DirectListingWithCapitalRaiseMessage) == 48, "DLCR message size is incorrect");

} // namespace itch
