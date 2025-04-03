#include "../messages/stockTradingAction.hpp"
#include "handlers.hpp"

namespace itch  {
void handleStockTradingAction(const char* buffer, size_t size, int log_fd) {
    // Validate buffer size
    if (size < sizeof(itch::StockTradingActionMessage)) {
        std::cerr << "Error: Invalid buffer size for StockTradingAction message" << std::endl;
        return;
    }

    // Parse message using PackedParser
    itch::StockTradingActionMessage msg = itch::PackedParser<itch::StockTradingActionMessage>::parse(buffer, size);

    char csv_row[256];

    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock_symbol);
    
    // Add trading state and reason
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%c,%s", msg.trading_state, msg.reason);

    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }
}

} // namespace handlers 