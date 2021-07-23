/**
 * @file interface.h
 * @brief The hook procedure interface.
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

#include "mod/interface.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>


namespace game {

/**
 * @interface Hook
 * @brief The hook procedure interface.
 * @details The framework is: Trampoline -> Detour -> Callback.
 */
class Hook : public Mod {
public:
    void Enable() override;

    void Disable() override;

protected:
    /**
     * @brief The trampoline used to jump to a detour function.
     * @details
     * An example:
     * @code
     * Trampoline{ .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 } },
     *             .jmp_offset_pos{ sizeof(JMP) },
     *             .jmp_inst_len{ JMP_LEN } };
     * @endcode
     */
    struct Trampoline {
        //! The machine code for hook.
        std::vector<std::byte> code;

        //! The data offset of the jump offset to the destination in `code` array.
        std::size_t jmp_offset_pos;

        //! The length of the jump instruction.
        std::size_t jmp_inst_len;
    };

    //! Get the origin bytes before hooking.
    std::span<const std::byte> OriginBytes() const noexcept;

    //! Get the trampoline to a detour function.
    virtual Trampoline HookBytes() const noexcept = 0;

    //! Get the source address.
    virtual std::intptr_t From() const noexcept = 0;

    //! Get the destination address.
    virtual std::intptr_t To() const noexcept = 0;

    //! Get the return address of the callback function. It's optional.
    virtual std::optional<std::intptr_t> JumpRet() const noexcept;

private:
    std::vector<std::byte> origin_bytes_{};
};

}  // namespace game