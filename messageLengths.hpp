#pragma once
#include <cstddef>
#include <array>

namespace itch {

constexpr std::array<size_t, 256> buildMessageLengthTable() {
    std::array<size_t, 256> lengths{};

    // Core message types
    lengths['S'] = 12;  // System Event Message
    lengths['R'] = 39;  // Stock Directory
    lengths['H'] = 20;  // Stock Trading Action
    lengths['Y'] = 20;  // Reg SHO
    lengths['L'] = 26;  // Market Participant Position
    lengths['V'] = 35;  // MWCB Decline Level
    lengths['W'] = 12;  // MWCB Status
    lengths['K'] = 28;  // IPO Quoting Period
    lengths['J'] = 32;  // LULD Auction Collar
    lengths['h'] = 21;  // Operational Halt

    lengths['A'] = 36;  // Add Order - No MPID
    lengths['F'] = 40;  // Add Order - With MPID
    lengths['E'] = 30;  // Order Executed
    lengths['C'] = 35;  // Order Executed With Price
    lengths['X'] = 23;  // Order Cancel
    lengths['D'] = 19;  // Order Delete
    lengths['U'] = 35;  // Order Replace
    lengths['P'] = 43;  // Trade (non-cross)
    lengths['Q'] = 39;  // Cross Trade
    lengths['B'] = 19;  // Broken Trade
    lengths['I'] = 50;  // NOII
    lengths['N'] = 20;  // Retail Price Indicator
    lengths['O'] = 48;  // DLCR Price Discovery

    return lengths;
}

constexpr std::array<size_t, 256> messageLengthTable = buildMessageLengthTable();

inline size_t getMessageLength(char messageType) {
    return messageLengthTable[static_cast<unsigned char>(messageType)];
}

} // namespace itch
