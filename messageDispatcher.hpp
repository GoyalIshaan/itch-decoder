#pragma once

#include <functional>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include "handlers.hpp" // Include all handler declarations

namespace itch {

// Define a type for a handler function
using HandlerFunction = void (*)(const char* buffer, size_t length, int log_fd);

// Declare the handler map creation function
constexpr std::array<HandlerFunction, 256> createHandlerMap();

// Declare the global handler map
extern const std::array<HandlerFunction, 256> handlerMap;

// Declare the dispatch function (inline definition will be in the .cpp)
void dispatch(char messageType, const char* buffer, size_t length, int log_fd);

} // namespace itch