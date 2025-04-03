#include "messages/orderExecuted.hpp"
#include "handlers.hpp"

namespace itch {

void handleOrderExecuted(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(OrderExecutedMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Order Executed message\n";
        return;
    }

    OrderExecutedMessage msg = itch::PackedParser<OrderExecutedMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.order_ref_number));

    // Write executed shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.executed_shares));

    // Write match number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu\n", fast_ntohll(msg.match_number));

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 