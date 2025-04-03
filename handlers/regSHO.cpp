#include "messages/regSHO.hpp"
#include "handlers.hpp"

namespace itch {

void handleRegSHO(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(RegSHOMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for RegSHO message\n";
        return;
    }

    RegSHOMessage msg = itch::PackedParser<RegSHOMessage>::parse(buffer, length);
    
    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock_symbol);

    // Write RegSHO action
    csv_row[offset++] = msg.reg_sho_action;
    csv_row[offset++] = '\n';

    // Write to file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace itch 