#include "../messages/addOrderWithMPID.hpp"
#include "handlers.hpp"

namespace itch {

void handleAddOrderWithMPID(const char* buffer, size_t size, int log_fd) {
    // Validate buffer size
    if (size < sizeof(AddOrderAttributionMessage)) {
        std::cerr << "Error: Invalid buffer size for AddOrderWithMPID message. "
                  << "Got " << size << " bytes, expected " << sizeof(AddOrderAttributionMessage) 
                  << " bytes" << std::endl;
        return;
    }

    // Parse message using PackedParser
    AddOrderAttributionMessage msg = PackedParser<AddOrderAttributionMessage>::parse(buffer, size);

    // Generate CSV row
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);
    
    // Add order reference number
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.order_ref_number));
    
    // Add buy/sell indicator
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%c,", msg.buy_sell_indicator);
    
    // Add shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.shares));
    
    // Add stock symbol (trimmed)
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);
    
    // Add price (fixed point 6.4 format)
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.price)/10000);
    
    // Add MPID (trimmed)
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(4), msg.attribution);

    // Write to log file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace handlers 