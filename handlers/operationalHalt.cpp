#include "messages/operationalHalt.hpp"
#include "handlers.hpp"

namespace itch {

void handleOperationalHalt(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(OperationalHaltMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for Operational Halt message\n";
        return;
    }

    OperationalHaltMessage msg = itch::PackedParser<OperationalHaltMessage>::parse(buffer, length);
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

   offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock);

    // Write market code and halt action
    csv_row[offset++] = msg.market_code;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.halt_action;
    csv_row[offset++] = '\n';

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 