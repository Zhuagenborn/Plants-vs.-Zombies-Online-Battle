/**
 * @file ip_addr.h
 * @brief IPv4 and IPv6 address.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-08-01
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#pragma once

#define _WINSOCKAPI_

#include <winsock2.h>
#include <ws2tcpip.h>

#include <concepts>
#include <cstddef>
#include <string_view>

namespace net {

//! The interface of IP address.
class IpAddr {
public:
    virtual ~IpAddr() noexcept = default;

    //! Get the IP version.
    virtual int Version() const noexcept = 0;

    //! Get the size of socket address.
    virtual std::size_t Size() const noexcept = 0;

    //! Get the socket address.
    virtual const sockaddr* Raw() const noexcept = 0;
};

//! The IPv4 address.
class Ipv4Addr final : public IpAddr {
public:
    static constexpr int version{ AF_INET };

    static constexpr std::string_view loop_back{ "127.0.0.1" };

    static constexpr std::string_view any{ "0.0.0.0" };

    using RawType = sockaddr_in;

    explicit Ipv4Addr(const sockaddr_in& addr) noexcept;

    explicit Ipv4Addr(std::string_view ip, std::uint16_t port);

    int Version() const noexcept override;

    std::size_t Size() const noexcept override;

    const sockaddr* Raw() const noexcept override;

private:
    sockaddr_in addr_{};
};

//! The IPv6 address.
class Ipv6Addr final : public IpAddr {
public:
    static constexpr int version{ AF_INET6 };

    static constexpr std::string_view loop_back{ "::1" };

    static constexpr std::string_view any{ "::" };

    using RawType = sockaddr_in6;

    explicit Ipv6Addr(const sockaddr_in6& addr) noexcept;

    explicit Ipv6Addr(std::string_view ip, std::uint16_t port);

    int Version() const noexcept override;

    std::size_t Size() const noexcept override;

    const sockaddr* Raw() const noexcept override;

private:
    sockaddr_in6 addr_{};
};

template <typename T>
concept ValidIpAddr =
    std::derived_from<T, IpAddr> && !std::same_as<T, IpAddr> && requires(T) {
    { T::version } -> std::convertible_to<int>;
    typename T::RawType;
};

}  // namespace net