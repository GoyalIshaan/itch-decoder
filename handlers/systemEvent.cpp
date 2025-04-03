#include "messages/systemEvent.hpp"
#include "handlers.hpp"

namespace itch {

void handleSystemEvent(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(SystemEventMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for System Event message\n";
        return;
    }

   SystemEventMessage msg = PackedParser<SystemEventMessage>::parse(buffer, length);

    char csv_row[256]; // Ensure this is large enough
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    csv_row[offset++] = msg.event_code;
    csv_row[offset++] = '\n';

    ssize_t written = write(log_fd, csv_row, offset);
    
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 