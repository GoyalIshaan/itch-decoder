#include "messages/tradeCross.hpp"
#include "handlers.hpp"

namespace itch {

void handleTradeCross(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(CrossTradeMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Cross Trade message\n";
        return;
    }

    CrossTradeMessage msg = itch::PackedParser<CrossTradeMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.shares));

    // Write stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // Write cross price
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.cross_price)/10000);

    // Write match number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.match_number));

    // Write cross type
    csv_row[offset++] = msg.cross_type;
    csv_row[offset++] = '\n';

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 