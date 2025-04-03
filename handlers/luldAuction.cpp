#include "messages/luldAuction.hpp"
#include "handlers.hpp"

namespace itch {

void handleLULDAuction(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(LULDAuctionCollarMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for LULD Auction Collar message\n";
        return;
    }

    LULDAuctionCollarMessage msg = itch::PackedParser<LULDAuctionCollarMessage>::parse(buffer, length);
    
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // prices and extension
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.reference_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.upper_auction_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.lower_auction_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u\n", fast_ntohl(msg.auction_extension));

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 