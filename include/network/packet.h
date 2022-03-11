/**
 * @file packet.h
 * @brief The network packet.
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

#include "socket/tcp.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>


namespace net {

/**
 * @brief The header of a network packet.
 */
struct alignas(std::int32_t) Header {
    //! The size of the following data.
    std::size_t size;
};

/**
 * @brief The network packet.
 */
class Packet {
public:
    /**
     * @brief Receive a packet
     *
     * @tparam ADDR     A valid IP address.
     * @param socket    A socket.
     * @return A packet.
     */
    template <ValidIpAddr ADDR>
    static Packet Recv(const TcpSocket<ADDR>& socket) {
        Packet pkg{};

        Header header{};
        socket.Recv({ reinterpret_cast<std::byte*>(&header), sizeof(header) });
        pkg.Write({ reinterpret_cast<std::byte*>(&header), sizeof(header) });

        std::unique_ptr<std::byte[]> body{ new std::byte[header.size]{} };
        socket.Recv({ body.get(), header.size });
        pkg.Write({ body.get(), header.size });
        return pkg;
    }

    /**
     * @brief Send the packet.
     *
     * @tparam ADDR     An IP address.
     * @param socket    A socket.
     */
    template <ValidIpAddr ADDR>
    void Send(const TcpSocket<ADDR>& socket) {
        socket.Send(buffer_);
    }

    /**
     * @brief Write data.
     *
     * @param data  A buffer.
     * @return The packet size.
     */
    std::size_t Write(std::span<const std::byte> data) noexcept;

    /**
     * @brief Read data.
     *
     * @return A buffer.
     */
    std::span<const std::byte> Read() noexcept;

private:
    std::vector<std::byte> buffer_{};
};

}  // namespace net