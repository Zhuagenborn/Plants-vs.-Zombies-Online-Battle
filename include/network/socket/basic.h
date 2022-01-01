/**
 * @file basic.h
 * @brief Basic socket operations.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-07-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#pragma once

#define _WINSOCKAPI_

#include "network/ip_addr.h"

#include "system/windows_error.h"

#include <winsock2.h>

#include <memory>
#include <stdexcept>
#include <string_view>


namespace net {

/**
 * @brief Check if the data is too large to be sent or received.
 *
 * @param size  The data size.
 * @param msg   An error message.
 *
 * @exception std::overflow_error   The data is too large.
 */
void CheckSizeLimit(const std::size_t size, const std::string_view msg);

/**
 * @interface Socket
 * @brief The socket interface.
 *
 * @tparam ADDR An IP address.
 */
template <ValidIpAddr ADDR>
class Socket {
public:
    /**
     * @brief Construct an invalid socket.
     */
    Socket() noexcept;

    /**
     * @brief Construct a socket from a low-level handle.
     *
     * @param id    A socket handle.
     */
    Socket(const SOCKET id) noexcept;

    Socket(Socket&& that) noexcept;

    Socket& operator=(Socket&& that) & noexcept;

    Socket(const Socket&) = delete;

    Socket& operator=(const Socket&) = delete;

    //! Check if the socket is valid.
    bool Valid() const noexcept;

    //! Get the low-level socket handle.
    SOCKET ID() const noexcept;

    /**
     * @brief Get the IP address.
     *
     * @exception std::runtime_error    The IP address is empty.
     */
    const ADDR& Addr() const;

    //! Set An IP address.
    void SetAddr(const ADDR& addr) noexcept;

    /**
     * @brief Bind the IP address set by `SetAddr` to the socket.
     *
     * @exception sys::WindowsError The operation failed.
     */
    void Bind() const;

    //! Close the socket.
    void Close() noexcept;

protected:
    virtual ~Socket() noexcept;

    SOCKET id_{ INVALID_SOCKET };

private:
    std::unique_ptr<ADDR> addr_{};
};


template <ValidIpAddr ADDR>
Socket<ADDR>::Socket() noexcept = default;

template <ValidIpAddr ADDR>
Socket<ADDR>::Socket(const SOCKET id) noexcept : id_{ id } {}

template <ValidIpAddr ADDR>
Socket<ADDR>::Socket(Socket&& that) noexcept :
    id_{ that.id_ }, addr_{ std::move(that.addr_) } {
    that.id_ = INVALID_SOCKET;
}

template <ValidIpAddr ADDR>
Socket<ADDR>& Socket<ADDR>::operator=(Socket&& that) & noexcept {
    id_ = that.id_;
    addr_ = std::move(that.addr_);
    that.id_ = INVALID_SOCKET;
    return *this;
}

template <ValidIpAddr ADDR>
Socket<ADDR>::~Socket() noexcept {
    Close();
}

template <ValidIpAddr ADDR>
bool Socket<ADDR>::Valid() const noexcept {
    return id_ != INVALID_SOCKET;
}

template <ValidIpAddr ADDR>
SOCKET Socket<ADDR>::ID() const noexcept {
    return id_;
}

template <ValidIpAddr ADDR>
const ADDR& Socket<ADDR>::Addr() const {
    if (addr_ == nullptr) {
        throw std::runtime_error{ "The IP address is null." };
    }

    return *addr_;
}


template <ValidIpAddr ADDR>
void Socket<ADDR>::SetAddr(const ADDR& addr) noexcept {
    addr_ = std::make_unique<ADDR>(addr);
}

template <ValidIpAddr ADDR>
void Socket<ADDR>::Bind() const {
    if (bind(id_, Addr().Raw(), Addr().Size()) == SOCKET_ERROR) {
        sys::ThrowWsaLastError();
    }
}


template <ValidIpAddr ADDR>
void Socket<ADDR>::Close() noexcept {
    if (Valid()) {
        closesocket(id_);
        id_ = INVALID_SOCKET;
    }
}

}  // namespace net