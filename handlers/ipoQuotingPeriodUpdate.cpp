#include "messages/ipoQuotingPeriodUpdate.hpp"
#include "handlers.hpp"

namespace itch {

void handleIPOQuotingPeriodUpdate(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(IPOQuotingMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for IPO Quoting Period Update message\n";
        return;
    }

    IPOQuotingMessage msg = itch::PackedParser<IPOQuotingMessage>::parse(buffer, length);

    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // release time
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.release_time));

    // qualifier
    csv_row[offset++] = msg.qualifier;
    csv_row[offset++] = ',';

    // ipo price
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u\n", fast_ntohl(msg.ipo_price)/10000);

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 