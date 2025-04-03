#include "messages/tradeNonCross.hpp"
#include "handlers.hpp"

namespace itch {

void handleTradeNonCross(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(TradeMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Trade Non-Cross message\n";
        return;
    }

    TradeMessage msg = itch::PackedParser<TradeMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.order_ref_number));

    // Write buy/sell indicator
    csv_row[offset++] = msg.buy_sell_indicator;
    csv_row[offset++] = ',';

    // Write shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.shares));

    // Write stock symbol   
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // Write price
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.price)/10000);

    // Write match number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu\n", fast_ntohll(msg.match_number));

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 