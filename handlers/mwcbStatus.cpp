#include "messages/mwcbStatus.hpp"
#include "handlers.hpp"

namespace itch {

void handleMWCBStatus(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(MWCBStatusMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for MWCB Status message\n";
        return;
    }

    MWCBStatusMessage msg = itch::PackedParser<MWCBStatusMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write breached level
    csv_row[offset++] = msg.breached_level;
    csv_row[offset++] = '\n';

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 