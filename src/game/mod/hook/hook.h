/**
 * @file hook.h
 * @brief Hook procedures.
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
#include <optional>
#include <string_view>


namespace game::mod::hook {

/**
 * @brief The hook procedure before loading an online level.
 */
class BeforeLoadLevel : public Hook {
public:
    std::string_view Name() const noexcept;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

private:
    static void __stdcall Detour() noexcept;

    static void __stdcall Callback() noexcept;
};

/**
 * @brief The hook procedure after loading an online level.
 */
class AfterLoadLevel : public Hook {
public:
    std::string_view Name() const noexcept;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

    std::optional<std::intptr_t> JumpRet() const noexcept override;

private:
    static constexpr std::size_t DETOUR_LEN{ 0x24 };

    static void __stdcall Detour() noexcept;

    static void __stdcall Callback() noexcept;
};

/**
 * @brief Disable the running menu.
 */
class DisableRuntimeMenu : public Hook {
public:
    std::string_view Name() const noexcept override;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;
};

/**
 * @brief The slot initializer.
 */
class InitSlots : public Hook {
public:
    InitSlots() noexcept;

    std::string_view Name() const noexcept override;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

    std::optional<std::intptr_t> JumpRet() const noexcept override;

private:
    static constexpr std::size_t DETOUR_LEN{ 0x1A };

    static void __stdcall SetSlot(Slot& slot) noexcept;

    static void __stdcall Detour() noexcept;

    static bool initialized_;
};

/**
 * @brief The hook procedure at the end of a level.
 */
class LevelEnd : public Hook {
public:
    std::string_view Name() const noexcept override;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

    std::optional<std::intptr_t> JumpRet() const noexcept override;

private:
    static constexpr std::size_t DETOUR_LEN{ 0x14 };

    static void __stdcall Callback() noexcept;

    static void __stdcall Detour() noexcept;
};

/**
 * @brief The hook procedure when creating a zombie.
 */
class CreateZombie : public Hook {
public:
    std::string_view Name() const noexcept;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

    std::optional<std::intptr_t> JumpRet() const noexcept override;

private:
    static constexpr std::size_t ZOMBIE_DETOUR_LEN{ 0x20 };

    static void __stdcall ZombieDetour() noexcept;

    static void __stdcall PlantDetour() noexcept;

    static void __stdcall ZombieCallback(const std::int32_t pos_x,
                                         const std::int32_t pos_y,
                                         const std::int32_t id) noexcept;
};

/**
 * @brief The hook procedure when creating a plant.
 */
class CreatePlant : public Hook {
public:
    std::string_view Name() const noexcept;

protected:
    Trampoline HookBytes() const noexcept override;

    std::intptr_t From() const noexcept override;

    std::intptr_t To() const noexcept override;

    std::optional<std::intptr_t> JumpRet() const noexcept override;

private:
    static constexpr std::size_t DETOUR_LEN{ 0x21 };

    static void __stdcall Detour() noexcept;

    static void __stdcall Callback(const std::int32_t pos_x,
                                   const std::int32_t pos_y,
                                   const std::int32_t id) noexcept;
};

}  // namespace game::mod::hook