#pragma once
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace itch {

template<typename T>
class PackedParser {
public:
    static_assert(std::is_trivially_copyable<T>::value, "Parsed type must be trivially copyable");
    static_assert(std::is_standard_layout<T>::value, "Parsed type must have standard layout");

    static T parse(const char* buffer, size_t length) {
        if (!buffer || length < sizeof(T)) {
            throw std::invalid_argument("Buffer too small for message struct.");
        }

        T msg;
        std::memcpy(&msg, buffer, sizeof(T));
        return msg;
    }
};

} // namespace itch
