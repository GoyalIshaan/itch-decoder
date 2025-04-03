#include "messages/replaceOrder.hpp"
#include "handlers.hpp" 

namespace itch {
void handleOrderReplace(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(OrderReplaceMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Order Replace message\n";
        return;
    }

    OrderReplaceMessage msg = itch::PackedParser<OrderReplaceMessage>::parse(buffer, length);
    
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write original order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.original_order_ref));

    // Write new order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.new_order_ref));

    // Write shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.shares));

    // Write price
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u\n", fast_ntohl(msg.price)/10000);

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 