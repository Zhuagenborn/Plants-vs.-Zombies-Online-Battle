/**
 * @file listener.h
 * @brief The TCP server.
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

#include "ip_addr.h"
#include "socket/tcp.h"

#include "system/windows_error.h"

#include <winsock2.h>


namespace net {

/**
 * @brief The TCP server.
 *
 * @tparam ADDR An IP address.
 */
template <ValidIpAddr ADDR>
class Listener final {
public:
    /**
     * @brief Bind an IP address to the server.
     *
     * @param addr  An IP address.
     *
     * @exception sys::WindowsError The operation failed.
     */
    void Bind(const ADDR& addr);

    /**
     * @brief Start to listen.
     *
     * @exception sys::WindowsError The operation failed.
     */
    void Listen();

    /**
     * @brief Close the server.
     */
    void Close() noexcept;

    /**
     * @brief Accept a connection.
     *
     * @exception sys::WindowsError The operation failed.
     */
    TcpSocket<ADDR> Accept();

private:
    TcpSocket<ADDR> socket_{};
};


template <ValidIpAddr ADDR>
void Listener<ADDR>::Bind(const ADDR& addr) {
    socket_.SetAddr(addr);
    socket_.Bind();
}


template <ValidIpAddr ADDR>
void Listener<ADDR>::Listen() {
    if (listen(socket_.ID(), SOMAXCONN) == SOCKET_ERROR) {
        sys::ThrowWsaLastError();
    }
}


template <ValidIpAddr ADDR>
void Listener<ADDR>::Close() noexcept {
    socket_.Close();
}

template <ValidIpAddr ADDR>
TcpSocket<ADDR> Listener<ADDR>::Accept() {
    typename ADDR::RawType addr{};
    int size{ sizeof(addr) };

    if (const auto new_id{
            accept(socket_.ID(), reinterpret_cast<sockaddr*>(&addr), &size) };
        new_id != INVALID_SOCKET) {
        return { new_id };
    } else {
        sys::ThrowWsaLastError();
    }
}

}  // namespace net