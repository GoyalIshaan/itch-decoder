#include "../messages/stockDirectory.hpp"
#include "handlers.hpp"

namespace itch {
void handleStockDirectory(const char* buffer, size_t length, int log_fd) {
    // Validate buffer size

    if (__builtin_expect(!buffer || length < sizeof(itch::StockDirectoryMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for StockDirectory message\n";
        return;
    }
    // Parse message using PackedParser
    itch::StockDirectoryMessage msg = itch::PackedParser<itch::StockDirectoryMessage>::parse(buffer, length);

    char csv_row[256];
    size_t offset = handleHeader(csv_row, sizeof(csv_row), msg);

    // Write stock symbol
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%.*s,", static_cast<int>(8), msg.stock_symbol);
    
    // Add market category and financial status
    csv_row[offset++] = msg.market_category;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.financial_status;
    csv_row[offset++] = ',';

    // Add round lot size and round lots only flag
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.round_lot_size));
    csv_row[offset++] = msg.round_lots_only;
    csv_row[offset++] = ',';
    
    csv_row[offset++] = msg.issue_classification;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.issue_sub_type[0];
    csv_row[offset++] = msg.issue_sub_type[1];
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.authenticity;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.short_sale_threshold;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.ipo_flag;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.luld_tier;
    csv_row[offset++] = ',';
    csv_row[offset++] = msg.etp_flag;
    csv_row[offset++] = ',';

    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", fast_ntohl(msg.etp_leverage_factor));

    csv_row[offset++] = msg.inverse_indicator;
    csv_row[offset++] = '\n';

    // Write to log file
    ssize_t written = write(log_fd, csv_row, offset);
    if (__builtin_expect(written != (ssize_t)offset, 0)) {
        std::cerr << "Error writing to log file, wrote: " << written << " bytes, expected " << offset << "\n";
    }   
}

} // namespace itch