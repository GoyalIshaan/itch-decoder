#include "../messages/marketParticipantPosition.hpp"
#include "handlers.hpp"

namespace itch {

void handleMarketParticipantPosition(const char* buffer, size_t size, int log_fd) {
    // Validate buffer size
    if (size < sizeof(itch::MarketParticipantPositionMessage)) {
        std::cerr << "Error: Invalid buffer size for MarketParticipantPosition message";
        return;
    }

    MarketParticipantPositionMessage msg = itch::PackedParser<MarketParticipantPositionMessage>::parse(buffer, size);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // MPID
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(4), msg.mpid);

    // Stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);
    
    // Market maker flags and participant state
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%c,%c,%c", msg.primary_market_maker, msg.market_maker_mode, msg.participant_state);

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

}