#pragma once

#include <functional>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include "handlers.hpp" // Include all handlers

namespace itch {

// Define a type for a handler function
using HandlerFunction = void (*)(const char* buffer, size_t length, int log_fd);

// Define message handler with char as a first index
constexpr std::array<HandlerFunction, 256> createHandlerMap() {
    std::array<HandlerFunction, 256> handlerMap{};

    // System Event Message
    handlerMap['S'] = handleSystemEventMessage;
    // Stock Related Messages
    handlerMap['R'] = handleStockDirectory;
    handlerMap['H'] = handleTradingActionMessage;
    handlerMap['Y'] = handleRegShoMessage; // Reg SHO Short Sale Price Test Restricted Indicator
    handlerMap['L'] = handleMarketParticipantPositionMessage;
    handlerMap['V'] = handleMwcbDeclineLevelMessage;   // MWCB Decline Level
    handlerMap['W'] = handleMwcbStatusMessage;        // MWCB Status Message
    handlerMap['K'] = handleIpoQuotingPeriodUpdateMessage;
    handlerMap['J'] = handleLuldAuctionCollarMessage;  // LULD Auction Collar
    handlerMap['h'] = handleOperationalHaltMessage;
    // Add Order Message
    handlerMap['A'] = handleAddOrderNoMpidMessage;   // Add Order - No MPID
    handlerMap['E'] = handleAddOrderMpidMessage;      // Add Order - MPID
    // Modify Order Messages
    handlerMap['C'] = handleOrderExecutedWithPriceMessage;  // Order Executed With PriceMessage
    handlerMap['X'] = handleOrderCancelMessage;        // Order Cancel Message
    handlerMap['D'] = handleOrderDeleteMessage;        // Order Delete Message
    handlerMap['U'] = handleOrderReplaceMessage;       // Order Replace Message
    // Trade Messages
    handlerMap['P'] = handleTradeMessage;             // Trade Message (Non-Cross)
    handlerMap['Q'] = handleCrossTradeMessage;         // Cross Trade Message
    handlerMap['B'] = handleBrokenTradeMessage;        // Broken Trade / Order ExecutionMessage
    // Net Order Imbalance Indicator (NOII)Message
    handlerMap['I'] = handleNoiiMessage;
    // Retail Price Improvement Indicator(RPII)
    handlerMap['N'] = handleRetailPriceImprovementMessage;
    // Direct Listing with Capital Raise Price DiscoveryMessage
    handlerMap['O'] = handleDirectListingCapitalRaiseMessage;

    return handlerMap;
}

// Global message handler map
constexpr auto handlerMap = createHandlerMap();

// Dispatch function
inline void dispatch(char messageType, const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(handlerMap[messageType] != nullptr, 1)) {
        handlerMap[messageType](buffer, length, log_fd);
    } else {
        std::cerr << "Error: No handler registered for message type '" << messageType << "'\n";
    }
}

} // namespace itch