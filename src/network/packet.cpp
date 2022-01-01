#include "packet.h"


namespace net {

std::size_t Packet::Write(const std::span<const std::byte> data) noexcept {
    buffer_.reserve(buffer_.size() + data.size());
    for (const auto byte : data) {
        buffer_.push_back(byte);
    }

    return buffer_.size();
}

std::span<const std::byte> Packet::Read() noexcept {
    return buffer_;
}

}