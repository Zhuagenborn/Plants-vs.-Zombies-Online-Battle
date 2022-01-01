/**
 * @file state.h
 * @brief Running status.
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

#include "network/socket/tcp.h"

#include <memory>
#include <thread>


namespace game::state {

//! The player's role.
extern Role role;

//! The configuration.
extern Config cfg;

//! The network connection.
extern std::unique_ptr<net::TcpSocket<cfg::IpAddr>> conn;

/**
 * @brief The stoppable thread.
 */
struct StoppableThread {
    //! The thread handle.
    std::unique_ptr<std::jthread> thread;

    //! A stop source that can send a stop request.
    std::unique_ptr<std::stop_source> stop_src;
};

//! The network communication thread.
extern StoppableThread recv_thread;

}  // namespace game::state