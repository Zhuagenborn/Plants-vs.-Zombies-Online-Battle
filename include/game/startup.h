/**
 * @file startup.h
 * @brief The initializer.
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


namespace game {

/**
 * @brief The initializer.
 */
class Startup final {
public:
    /**
     * @brief Construct an initializer.
     *
     * @param role  A player's role.
     * @param cfg   A configuration.
     */
    Startup(const Role role, Config cfg) noexcept;

    ~Startup() noexcept;

    /**
     * @brief Initialize the game.
     */
    void Run();

    /**
     * @brief Release resources.
     */
    void Stop() noexcept;
};

}  // namespace game
