#pragma once

#include <cstddef>
#include <cstdint>
#include "packedParser.hpp"
#include "headerHandler.hpp"
#include <iostream>
#include <unistd.h>

namespace itch {
void handleSystemEvent(const char* buffer, size_t length, int log_fd);
void handleStockDirectory(const char* buffer, size_t length, int log_fd);
void handleStockTradingAction(const char* buffer, size_t length, int log_fd);
void handleRegSHO(const char* buffer, size_t length, int log_fd);
void handleMarketParticipantPosition(const char* buffer, size_t length, int log_fd);
void handleMWCBDeclineLevel(const char* buffer, size_t length, int log_fd);
void handleMWCBStatus(const char* buffer, size_t length, int log_fd);
void handleIPOQuotingPeriodUpdate(const char* buffer, size_t length, int log_fd);
void handleLULDAuction(const char* buffer, size_t length, int log_fd);
void handleOperationalHalt(const char* buffer, size_t length, int log_fd);
void handleAddOrderNoMPID(const char* buffer, size_t length, int log_fd);
void handleAddOrderWithMPID(const char* buffer, size_t length, int log_fd);
void handleOrderExecuted(const char* buffer, size_t length, int log_fd);
void handleOrderExecutedWithPrice(const char* buffer, size_t length, int log_fd);
void handleOrderCancel(const char* buffer, size_t length, int log_fd);
void handleOrderDelete(const char* buffer, size_t length, int log_fd);
void handleOrderReplace(const char* buffer, size_t length, int log_fd);
void handleTradeNonCross(const char* buffer, size_t length, int log_fd);
void handleTradeCross(const char* buffer, size_t length, int log_fd);
void handleBrokenTrade(const char* buffer, size_t length, int log_fd);
void handleNOII(const char* buffer, size_t length, int log_fd);
void handleRPII(const char* buffer, size_t length, int log_fd);
void handleDLCR(const char* buffer, size_t length, int log_fd);
inline uint32_t fast_ntohl(uint32_t net) { return ntohl(net); } // POSIX standard
inline uint64_t fast_ntohll(uint64_t net) { return ntohll(net); } // POSIX standard

} // namespace itch