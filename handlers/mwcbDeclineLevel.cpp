#include "messages/mwcbDeclineLevel.hpp"
#include "handlers.hpp"

namespace itch {

void handleMWCBDeclineLevel(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(MWCBDeclineLevelMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for MWCB Decline Level message\n";
        return;
    }

    MWCBDeclineLevelMessage msg = itch::PackedParser<MWCBDeclineLevelMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write price levels
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.level_1_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", fast_ntohll(msg.level_2_price)/10000);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu\n", fast_ntohll(msg.level_3_price)/10000);

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 