#include "messageDispatcher.hpp"

namespace itch {

// Define message handler with char as a first index
constexpr std::array<HandlerFunction, 256> createHandlerMap() {
    std::array<HandlerFunction, 256> handlerMap{};

    // System Event Message
    handlerMap['S'] = &handleSystemEvent;
    // Stock Related Messages
    handlerMap['R'] = &handleStockDirectory;
    handlerMap['H'] = &handleStockTradingAction;
    handlerMap['Y'] = &handleRegSHO; // Reg SHO Short Sale Price Test Restricted Indicator
    handlerMap['L'] = &handleMarketParticipantPosition;
    handlerMap['V'] = &handleMWCBDeclineLevel;   // MWCB Decline Level
    handlerMap['W'] = &handleMWCBStatus;        // MWCB Status Message
    handlerMap['K'] = &handleIPOQuotingPeriodUpdate;
    handlerMap['J'] = &handleLULDAuction;  // LULD Auction Collar
    handlerMap['h'] = &handleOperationalHalt;
    // Add Order Message
    handlerMap['A'] = &handleAddOrderNoMPID;   // Add Order - No MPID
    handlerMap['F'] = &handleAddOrderWithMPID; // Add Order - With MPID
    handlerMap['E'] = &handleOrderExecuted;      // Order Executed
    // Modify Order Messages
    handlerMap['C'] = &handleOrderExecutedWithPrice;  // Order Executed With PriceMessage
    handlerMap['X'] = &handleOrderCancel;        // Order Cancel Message
    handlerMap['D'] = &handleOrderDelete;        // Order Delete Message
    handlerMap['U'] = &handleOrderReplace;       // Order Replace Message
    // Trade Messages
    handlerMap['P'] = &handleTradeNonCross;             // Trade Message (Non-Cross)
    handlerMap['Q'] = &handleTradeCross;         // Cross Trade Message
    handlerMap['B'] = &handleBrokenTrade;        // Broken Trade / Order ExecutionMessage
    // Net Order Imbalance Indicator (NOII)Message
    handlerMap['I'] = &handleNOII;
    // Retail Price Improvement Indicator(RPII)
    handlerMap['N'] = &handleRPII;
    // Direct Listing with Capital Raise Price DiscoveryMessage
    handlerMap['O'] = &handleDLCR;

    return handlerMap;
}

// Global message handler map - definition
const std::array<HandlerFunction, 256> handlerMap = createHandlerMap();

// Dispatch function - definition
void dispatch(char messageType, const char* buffer, size_t length, int log_fd) {
    if (__builtin_expect(handlerMap[messageType] != nullptr, 1)) {
        handlerMap[messageType](buffer, length, log_fd);
    }
}

} // namespace itch