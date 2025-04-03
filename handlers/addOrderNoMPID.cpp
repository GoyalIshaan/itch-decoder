#include "messages/addOrderNoMPID.hpp"
#include "handlers.hpp"

namespace itch {
void handleAddOrderNoMPID(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(AddOrderNoMPIDMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Add Order No MPID message\n";
        return;
    }

    AddOrderNoMPIDMessage msg = PackedParser<AddOrderNoMPIDMessage>::parse(buffer, length);

    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);
    
    // Write order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.order_ref_number));

    // Write buy/sell indicator
    csv_row[offset++] = msg.buy_sell_indicator;
    csv_row[offset++] = ',';

    // Write shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.shares));

    // Copy Stock Symbol (Handle spaces as early as possible)
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);


    // Write price (in fixed-point format)
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u\n", fast_ntohl(msg.price));

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 