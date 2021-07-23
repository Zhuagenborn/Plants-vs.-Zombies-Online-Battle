/**
 * @file interface.h
 * @brief The modification interface.
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

#include <cstddef>
#include <cstdint>
#include <string_view>


namespace game {

/**
 * @interface Mod
 * @brief The modification interface.
 */
class Mod {
public:
    virtual ~Mod() noexcept;

    //! Enable the modification.
    virtual void Enable() = 0;

    //! Disable the modification.
    virtual void Disable();

    //! Get the unique name of modification.
    virtual std::string_view Name() const noexcept = 0;
};


namespace mod {

//! The base number of zombie IDs.
constexpr std::int32_t ZOMBIE_ID_BASE{ 0x3C };

/**
 * @brief The global base address.
 *
 * @warning
 * This value cannot be defined using `constexpr`.
 * When referring to it in a naked function defined using `__declspec(naked)`,
 * a `constexpr` variable cannot be converted into a constant number as normal.
 */
#define BASE_ADDR 0x006A9F38

/**
 * @brief The address of `CreateZombie` function.
 *
 * @warning
 * This value cannot be defined using `constexpr`.
 * When referring to it in a naked function defined using `__declspec(naked)`,
 * a `constexpr` variable cannot be converted into a constant number as normal.
 */
#define FUNC_CREATE_ZOMBIE_ADDR 0x0042A0F0

/**
 * @brief The address of `CreatePlant` function.
 *
 * @warning
 * This value cannot be defined using `constexpr`.
 * When referring to it in a naked function defined using `__declspec(naked)`,
 * a `constexpr` variable cannot be converted into a constant number as normal.
 */
#define FUNC_CREATE_PLANT_ADDR 0x0040D120

/**
 * @brief The address of `EndLevel` function.
 *
 * @warning
 * This value cannot be defined using `constexpr`.
 * When referring to it in a naked function defined using `__declspec(naked)`,
 * a `constexpr` variable cannot be converted into a constant number as normal.
 */
#define FUNC_END_LEVEL_ADDR 0x00413400

/**
 * @brief The ID of the online level.
 *
 * @warning
 * This value cannot be defined using `constexpr`.
 * When referring to it in a naked function defined using `__declspec(naked)`,
 * a `constexpr` variable cannot be converted into a constant number as normal.
 */
#define MOD_LEVEL_ID 0x46

/**
 * @brief The slot.
 * @details
 * Set a breakpoint at `0x004897B2`.
 * Each time the game is interrupted while loading a new level,
 * the 4-bit content of `[ESP]` will be the address of the slot structure that the game is displaying currently.
 */
struct Slot {
    std::byte unknown1[8];

    /**
     * @brief The X-coordinate on the screen.
     * @par Offset
     * 0x8
     */
    std::int32_t screen_pos_x;

    /**
     * @brief The Y-coordinate on the screen.
     * @par Offset
     * 0xC
     */
    std::int32_t screen_pos_y;

    std::byte unknown2[36];

    /**
     * @brief The item ID.
     * @details
     * If the type of the slot is zombie, this field is equal to the zombie ID plus `ZOMBIE_ID_BASE`.
     * If the type of the slot is plant, this field is equal to the plant ID.
     *
     * @par Offset
     * 0x34
     */
    std::int32_t id;
};

/**
 * @brief The planted plant in a level.
 */
struct PlantedPlant {
    std::byte unknown1[321];

    /**
     * @brief If the plant is invalid.
     * @details If this field is `true`, the plant will be removed.
     *
     * @par Offset
     * 0x141
     */
    bool invalid;

    std::byte unknown2[10];
};

/**
 * @brief Static plant information.
 * @par Binary Address
 * 0x0069F2B0
 */
struct Plant {
    /**
     * @brief The ID.
     * @par Offset
     * 0x0
     */
    std::int32_t id;

    std::byte unknown1[4];

    /**
     * @brief The image resource.
     * @par Offset
     * 0x8
     */
    std::int32_t ui;

    std::byte unknown2[4];

    /**
     * @brief The cost to plant.
     * @par Offset
     * 0x10
     */
    std::int32_t sun_cost;

    /**
     * @brief The recharge time.
     * @par Offset
     * 0x14
     */
    std::int32_t recharge_time;

    std::byte unknown3[12];

    /**
     * @brief The name.
     * @par Offset
     * 0x20
     */
    char* name;
};

/**
 * @brief Create a zombie.
 *
 * @param pos_x The X-coordinate of the target location.
 * @param pos_y The Y-coordinate of the target location.
 * @param id    A zombie ID.
 *
 * @bug This function sometimes raises an access exception.
 */
void CreateZombie(const std::int32_t pos_x, const std::int32_t pos_y,
                  const std::int32_t id) noexcept;

/**
 * @brief Create a plant.
 *
 * @param pos_x The X-coordinate of the target location.
 * @param pos_y The Y-coordinate of the target location.
 * @param id    A plant ID.
 */
void CreatePlant(const std::int32_t pos_x, const std::int32_t pos_y,
                 const std::int32_t id) noexcept;

/**
 * @brief Terminate the current level.
 */
void EndLevel() noexcept;

}  // namespace mod

}  // namespace game