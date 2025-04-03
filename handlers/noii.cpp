#include "messages/noii.hpp"
#include "handlers.hpp"

namespace itch {

void handleNOII(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(NOIIMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for NOII message\n";
        return;
    }

    NOIIMessage msg = itch::PackedParser<NOIIMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write paired shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.paired_shares));

    // Write imbalance shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.imbalance_shares));

    // Write imbalance direction
    csv_row[offset++] = msg.imbalance_direction;
    csv_row[offset++] = ',';

    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // Write prices
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.far_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.near_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.current_reference_price)/10000);

    // Write cross type
    csv_row[offset++] = msg.cross_type;
    csv_row[offset++] = ',';

    // Write price variation indicator
    csv_row[offset++] = msg.price_variation_indicator;
    csv_row[offset++] = '\n';

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 