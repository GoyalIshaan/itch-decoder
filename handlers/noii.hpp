#pragma once
#include "messages/noii.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include "packedParser.hpp"
namespace itch {

constexpr size_t STOCK_SYMBOL_SIZE = 8;
constexpr size_t TIMESTAMP_SIZE = 6;

inline uint16_t fast_ntohs(uint16_t net) { return ntohs(net); }   // POSIX standard
inline uint32_t fast_ntohl(uint32_t net) { return ntohl(net); } // POSIX standard
inline uint64_t fast_ntohll(uint64_t net) { return ntohll(net); } // POSIX standard

inline void handleNOII(const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(!buffer || length < sizeof(NOIIMessage), 0)) {
        std::cerr << "Error: Invalid buffer size for NOII message\n";
        return;
    }

    NOIIMessage msg = itch::PackedParser<NOIIMessage>::parse(buffer, length);
    
    // Pre-convert all fields in place
    msg.stock_locate = fast_ntohs(msg.stock_locate);
    msg.tracking_number = fast_ntohs(msg.tracking_number);
    msg.paired_shares = fast_ntohll(msg.paired_shares);
    msg.imbalance_shares = fast_ntohll(msg.imbalance_shares);
    msg.far_price = fast_ntohl(msg.far_price);
    msg.near_price = fast_ntohl(msg.near_price);
    msg.current_reference_price = fast_ntohl(msg.current_reference_price);

    // CSV Row Generation using minimal buffer manipulation
    char csv_row[256];
    size_t offset = 0;

    // Write fields in order
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", msg.stock_locate);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", msg.tracking_number);

    // Write timestamp
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%02X%02X%02X%02X%02X%02X,",
                      (unsigned char)msg.timestamp[0], (unsigned char)msg.timestamp[1],
                      (unsigned char)msg.timestamp[2], (unsigned char)msg.timestamp[3],
                      (unsigned char)msg.timestamp[4], (unsigned char)msg.timestamp[5]);

    // Write paired shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", msg.paired_shares);

    // Write imbalance shares
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%llu,", msg.imbalance_shares);

    // Write imbalance direction
    csv_row[offset++] = msg.imbalance_direction;
    csv_row[offset++] = ',';

    // Copy Stock Symbol (Handle spaces as early as possible)
    for (size_t i = 0; i < STOCK_SYMBOL_SIZE; ++i) {
        if (msg.stock[i] == ' ') {
            break;
        }
        csv_row[offset++] = msg.stock[i];
    }
    csv_row[offset++] = ',';

    // Write prices
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", msg.far_price);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", msg.near_price);
    offset += snprintf(csv_row + offset, sizeof(csv_row) - offset, "%u,", msg.current_reference_price);

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