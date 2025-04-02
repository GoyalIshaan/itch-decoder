#pragma once

#include <cstddef>
#include <cstdint>

namespace itch {

// System Event Message
void handleSystemEventMessage(const char* buffer, size_t length, int log_fd);
// Stock Related Messages
void handleStockDirectory(const char* buffer, size_t length, int log_fd);
void handleTradingActionMessage(const char* buffer, size_t length, int log_fd);
void handleRegShoMessage(const char* buffer, size_t length, int log_fd);
void handleMarketParticipantPositionMessage(const char* buffer, size_t length, int log_fd);
void handleMwcbDeclineLevelMessage(const char* buffer, size_t length, int log_fd);
void handleMwcbStatusMessage(const char* buffer, size_t length, int log_fd);
void handleIpoQuotingPeriodUpdateMessage(const char* buffer, size_t length, int log_fd);
void handleLuldAuctionCollarMessage(const char* buffer, size_t length, int log_fd);
void handleOperationalHaltMessage(const char* buffer, size_t length, int log_fd);
// Add Order Message
void handleAddOrderNoMpidMessage(const char* buffer, size_t length, int log_fd);
void handleAddOrderMpidMessage(const char* buffer, size_t length, int log_fd);
// Modify Order Messages
void handleOrderExecutedWithPriceMessage(const char* buffer, size_t length, int log_fd);
void handleOrderCancelMessage(const char* buffer, size_t length, int log_fd);
void handleOrderDeleteMessage(const char* buffer, size_t length, int log_fd);
void handleOrderReplaceMessage(const char* buffer, size_t length, int log_fd);
// Trade Messages
void handleTradeMessage(const char* buffer, size_t length, int log_fd);
void handleCrossTradeMessage(const char* buffer, size_t length, int log_fd);
void handleBrokenTradeMessage(const char* buffer, size_t length, int log_fd);
// Net Order Imbalance Indicator (NOII)Message
void handleNoiiMessage(const char* buffer, size_t length, int log_fd);
// Retail Price Improvement Indicator(RPII)
void handleRetailPriceImprovementMessage(const char* buffer, size_t length, int log_fd);
// Direct Listing with Capital Raise Price DiscoveryMessage
void handleDirectListingCapitalRaiseMessage(const char* buffer, size_t length, int log_fd);
}