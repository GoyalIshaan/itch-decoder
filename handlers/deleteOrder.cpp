#include "messages/deleteOrder.hpp"
#include "handlers.hpp"

namespace itch {

void handleOrderDelete(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(OrderDeleteMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Order Delete message\n";
        return;
    }

    OrderDeleteMessage msg = itch::PackedParser<OrderDeleteMessage>::parse(buffer, length);

    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // order ref number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu\n", fast_ntohll(msg.order_ref_number));

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 