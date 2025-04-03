#include "messages/dlcr.hpp"
#include "handlers.hpp"

namespace itch {

void handleDLCR(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(DirectListingWithCapitalRaiseMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for DLCR message\n";
        return;
    }

    DirectListingWithCapitalRaiseMessage msg = itch::PackedParser<DirectListingWithCapitalRaiseMessage>::parse(buffer, length);

    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

   // stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // open eligibility status
    csv_row[offset++] = msg.open_eligibility_status;
    csv_row[offset++] = ',';

    // prices and time
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.min_allowable_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.max_allowable_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.near_execution_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.near_execution_time));
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.lower_price_collar)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u\n", fast_ntohl(msg.upper_price_collar)/10000);

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 