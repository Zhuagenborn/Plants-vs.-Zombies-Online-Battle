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

/**
 * @interface IpAddr
 * @brief The interface of IP address.
 */
class IpAddr {
public:
    //! Get the IP version.
    virtual int Version() const noexcept = 0;

    //! Get the size of socket address.
    virtual std::size_t Size() const noexcept = 0;

    //! Get the socket address.
    virtual const sockaddr* Raw() const noexcept = 0;

private:
    //! A socket library initializer.
    struct Initializer {
        Initializer();
        ~Initializer();
    };

    static Initializer initializer_;
};

/**
 * @brief The IPv4 address.
 */
class Ipv4Addr final : public IpAddr {
public:
    static constexpr int VERSION{ AF_INET };

    static constexpr std::string_view LOOP_BACK{ "127.0.0.1" };

    static constexpr std::string_view ANY{ "0.0.0.0" };

    using RawType = sockaddr_in;

    explicit Ipv4Addr(const sockaddr_in& addr) noexcept;

    explicit Ipv4Addr(const std::string_view ip,
                      const std::uint16_t port) noexcept;

    int Version() const noexcept override;

    std::size_t Size() const noexcept override;

    const sockaddr* Raw() const noexcept override;

private:
    sockaddr_in addr_{};
};

/**
 * @brief The IPv6 address.
 */
class Ipv6Addr final : public IpAddr {
public:
    static constexpr int VERSION{ AF_INET6 };

    static constexpr std::string_view LOOP_BACK{ "::1" };

    static constexpr std::string_view ANY{ "::" };

    using RawType = sockaddr_in6;

    explicit Ipv6Addr(const sockaddr_in6& addr) noexcept;

    explicit Ipv6Addr(const std::string_view ip,
                      const std::uint16_t port) noexcept;

    int Version() const noexcept override;

    std::size_t Size() const noexcept override;

    const sockaddr* Raw() const noexcept override;

private:
    sockaddr_in6 addr_{};
};

/**
 * @concept ValidIpAddr
 * @brief A valid IP address.
 *
 * @tparam T    `Ipv4Addr` or `Ipv6Addr`.
 */
template <typename T>
concept ValidIpAddr =
    std::derived_from<T, IpAddr> && !std::same_as<T, IpAddr> && requires(T) {
    { T::VERSION } -> std::convertible_to<int>;
    typename T::RawType;
};

}  // namespace net