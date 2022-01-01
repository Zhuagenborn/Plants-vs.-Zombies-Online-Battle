#define NOMINMAX

#include "socket/basic.h"

#include <limits>


namespace net {

void CheckSizeLimit(const std::size_t size, const std::string_view msg) {
    if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::overflow_error{ msg.data() };
    }
}

}