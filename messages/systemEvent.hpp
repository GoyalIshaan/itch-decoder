#pragma once
#include <cstdint>
#include <iostream>
#include <stdexcept>

constexpr int SYSTEM_EVENT_MSG_SIZE = 12;

using namespace std;
struct SystemEventMessage {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char event_code;
};

SystemEventMessage parseSystemEventMessage(const char* buffer, size_t length) {
    if (buffer == nullptr || length < SYSTEM_EVENT_MSG_SIZE) {
        throw invalid_argument("Invalid buffer size");
    }

    SystemEventMessage msg;

    msg.stock_locate = (static_cast<uint8_t>(buffer[1]) << 8) | (static_cast<uint8_t>(buffer[2]));
    msg.tracking_number = (static_cast<uint8_t>(buffer[3]) << 8) | (static_cast<uint8_t>(buffer[4]));

    msg.timestamp =
    (static_cast<uint64_t>(buffer[5])  << 40) |
    (static_cast<uint64_t>(buffer[6])  << 32) |
    (static_cast<uint64_t>(buffer[7])  << 24) |
    (static_cast<uint64_t>(buffer[8])  << 16) |
    (static_cast<uint64_t>(buffer[9])  << 8)  |
    (static_cast<uint64_t>(buffer[10]));

    msg.event_code = buffer[11];

    return msg;
}

