/**
 * @file tcp.h
 * @brief The TCP socket.
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

#include "basic.h"

#include "system/windows_error.h"

#include <cstddef>
#include <span>


namespace net {

/**
 * @brief The TCP socket.
 *
 * @tparam ADDR An IP address.
 */
template <ValidIpAddr ADDR>
class TcpSocket : public Socket<ADDR> {
public:
    using Socket<ADDR>::Socket;

    /**
     * @brief Construct a TCP socket.
     *
     * @exception sys::WindowsError The initialization failed.
     */
    TcpSocket();

    /**
     * @brief Connect to a server.
     *
     * @param addr  An IP address.
     *
     * @exception sys::WindowsError The operation failed.
     */
    void Connect(const ADDR& addr) const;

    /**
     * @brief Send data.
     *
     * @exception sys::WindowsError The operation failed.
     */
    std::size_t Send(const std::span<const std::byte> data) const;

    /**
     * @brief Receive data.
     *
     * @param buffer    A buffer storing data. The function tries to fill this buffer.
     *
     * @exception sys::WindowsError The operation failed.
     */
    std::size_t Recv(const std::span<std::byte> buffer) const;
};


template <ValidIpAddr ADDR>
TcpSocket<ADDR>::TcpSocket() :
    Socket<ADDR>{ socket(ADDR::VERSION, SOCK_STREAM, 0) } {
    if (this->id_ == INVALID_SOCKET) {
        sys::ThrowWsaLastError();
    }
}

template <ValidIpAddr ADDR>
void TcpSocket<ADDR>::Connect(const ADDR& addr) const {
    if (connect(this->id_, addr.Raw(), addr.Size()) == SOCKET_ERROR) {
        sys::ThrowWsaLastError();
    }
}


template <ValidIpAddr ADDR>
std::size_t TcpSocket<ADDR>::Send(const std::span<const std::byte> data) const {
    CheckSizeLimit(data.size_bytes(), "The size of data is too large to send.");

    if (const auto sent{ send(this->id_,
                              reinterpret_cast<const char*>(data.data()),
                              data.size_bytes(), 0) };
        sent != SOCKET_ERROR) {
        return static_cast<std::size_t>(sent);
    } else {
        sys::ThrowWsaLastError();
    }
}

template <ValidIpAddr ADDR>
std::size_t TcpSocket<ADDR>::Recv(const std::span<std::byte> buffer) const {
    CheckSizeLimit(buffer.size_bytes(),
                   "The size of buffer is too large to receive data.");

    if (const auto received{ recv(this->id_,
                                  reinterpret_cast<char*>(buffer.data()),
                                  buffer.size_bytes(), 0) };
        received != SOCKET_ERROR) {
        return received;
    } else {
        sys::ThrowWsaLastError();
    }
}

}  // namespace net