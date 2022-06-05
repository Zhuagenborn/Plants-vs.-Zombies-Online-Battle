#include "ip_addr.h"

#include "system/windows_error.h"

#pragma comment(lib, "ws2_32.lib")


namespace net {

namespace {

//! A socket library initializer.
struct Initializer {
    Initializer();
    ~Initializer() noexcept;
};

const Initializer initializer_{};

Initializer::Initializer() {
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        sys::ThrowWsaLastError();
    }
}

Initializer::~Initializer() noexcept {
    WSACleanup();
}

}  // namespace

Ipv4Addr::Ipv4Addr(const sockaddr_in& addr) noexcept : addr_{ addr } {}

Ipv4Addr::Ipv4Addr(const std::string_view ip, const std::uint16_t port) {
    addr_.sin_family = version;
    addr_.sin_port = htons(port);
    if (inet_pton(version, ip.data(), &addr_.sin_addr) != 1) {
        sys::ThrowWsaLastError();
    }
}

int Ipv4Addr::Version() const noexcept {
    return version;
}

std::size_t Ipv4Addr::Size() const noexcept {
    return sizeof(addr_);
}

const sockaddr* Ipv4Addr::Raw() const noexcept {
    return reinterpret_cast<const sockaddr*>(&addr_);
}


Ipv6Addr::Ipv6Addr(const sockaddr_in6& addr) noexcept : addr_{ addr } {}

Ipv6Addr::Ipv6Addr(const std::string_view ip, const std::uint16_t port) {
    addr_.sin6_family = version;
    addr_.sin6_port = htons(port);
    if (inet_pton(version, ip.data(), &addr_.sin6_addr) != 1) {
        sys::ThrowWsaLastError();
    }
}

int Ipv6Addr::Version() const noexcept {
    return version;
}

std::size_t Ipv6Addr::Size() const noexcept {
    return sizeof(addr_);
}

const sockaddr* Ipv6Addr::Raw() const noexcept {
    return reinterpret_cast<const sockaddr*>(&addr_);
}

}  // namespace net