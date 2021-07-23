/**
 * @file mod.h
 * @brief Modifications.
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

#include "interface.h"

#include <cstddef>
#include <cstdint>
#include <string_view>


namespace game::mod {

/**
 * @brief Set the amount of sun.
 */
class SetSunAmount : public Mod {
public:
    SetSunAmount(const std::size_t amount) noexcept;

    void Enable() override;

    std::string_view Name() const noexcept override;

private:
    const std::size_t amount_;
};

/**
 * @brief Disable automatic pause during playing.
 */
class DisableAutoPause : public Mod {
public:
    void Enable() override;

    std::string_view Name() const noexcept override;
};

/**
 * @brief Remove all planted plants when an online level begins.
 */
class RemoveDefaultPlants : public Mod {
public:
    void Enable() override;

    std::string_view Name() const noexcept override;
};

/**
 * @brief Allow to run multiple game processes simultaneously.
 */
class AllowMultiProcess : public Mod {
public:
    void Enable() override;

    /**
     * @brief Modify the game file to allow to run multiple processes simultaneously.
     *
     * @param file_path The path of `PlantsVsZombies.exe`.
     *
     * @exception std::invalid_argument The path is empty.
     */
    void Enable(const std::string_view file_path);

    std::string_view Name() const noexcept override;

private:
    static constexpr std::intptr_t RAW_OFFSET{ 0x00153F1B };
    static constexpr std::intptr_t VIRTUAL_ADDR{ 0x00553F1B };
};

}  // namespace game::mod