/**
 * @file net_packet.h
 * @brief Network packets and the receiver thread.
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

#include "config.h"

#include "network/packet.h"

#include <cstdint>
#include <thread>


namespace game::netpkg {

//! Types of packets.
enum class Type { NewPlant, NewZombie, LevelEnd };

/**
 * @brief The header of a packet.
 */
struct alignas(std::int32_t) Header : public net::Header {
    //! The type.
    Type pkt_type;

    //! The player's role.
    Role role;
};

/**
 * @brief The packet storing a creation event.
 */
struct alignas(std::int32_t) NewItem : public Header {
    //! The X-coordinate of the target location.
    std::int32_t pos_x;

    //! The X-coordinate of the target location.
    std::int32_t pos_y;

    //! The item ID.
    std::int32_t id;
};

/**
 * @brief Process packets.
 *
 * @param packet    A packet.
 *
 * @exception std::invalid_argument An unknown packet type.
 */
void Process(const Header* const packet);

/**
 * @brief The receiver thread.
 *
 * @param stop_token    A stop token that can stop the thread.
 */
void RecvLoop(const std::stop_token stop_token) noexcept;

/**
 * @brief Stop the receiver thread.
 *
 * @param wait  Whether to wait for the thread to end.
 */
void StopRecvLoop(bool wait) noexcept;

}  // namespace game::netpkg