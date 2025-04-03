#pragma once
#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>

namespace itch {
template <typename T>
inline int handleHeader(char* buffer, size_t buffer_size, const T& header) {
    char* current = buffer;
    // Message Type
    *current++ = header.message_type;

    // Stock Locate (Host Endian)
    uint16_t stock_locate_host = ntohs(header.stock_locate);
    current += snprintf(current, buffer_size - (current - buffer), "%u,", stock_locate_host);

    // Tracking Number (Host Endian)
    uint16_t tracking_number_host = ntohs(header.tracking_number);
    current += snprintf(current, buffer_size - (current - buffer), "%u,", tracking_number_host);

    // Timestamp (Manual Hex Formatting - Faster)
    for (int i = 0; i < 6; ++i) {
        unsigned char byte = header.timestamp[i];
        int high_nibble = byte >> 4;
        int low_nibble = byte & 0x0F;
        *current++ = (high_nibble < 10) ? ('0' + high_nibble) : ('A' + high_nibble - 10);
        *current++ = (low_nibble < 10) ? ('0' + low_nibble) : ('A' + low_nibble - 10);
    }
    *current++ = ',';

    return current - buffer;
}
} // namespace itch