/**
 * @file memory.h
 * @brief Machine code and memory operations.
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

#include <Windows.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>


namespace sys {

//! The operation code of long `jmp` instruction.
constexpr std::byte JMP{ std::byte{ 0xE9 } };

//! The operation code of short `jmp` instruction.
constexpr std::byte SHORT_JMP{ std::byte{ 0xEB } };

//! The operation code of `call` instruction.
constexpr std::byte CALL{ std::byte{ 0xE8 } };

//! The operation code of `ret` instruction.
constexpr std::byte RET{ std::byte{ 0xC3 } };

//! The operation code of `nop` instruction.
constexpr std::byte NOP{ std::byte{ 0x90 } };

//! The length of long `jmp` instruction.
constexpr std::size_t JMP_LEN{ 5 };

//! The length of `call` instruction.
constexpr std::size_t CALL_LEN{ JMP_LEN };

/**
 * @brief Check if an address is `nullptr`.
 *
 * @param addr  An address.
 *
 * @exception std::invalid_argument The address is `nullptr`.
 */
void CheckNullPointer(const std::intptr_t addr);

/**
 * @brief Make a part of memory writable.
 *
 * @param addr  A base address.
 * @param size  The size.
 *
 * @exception WindowsError  The operation failed.
 */
void SetMemoryWritable(const std::intptr_t addr, const std::size_t size);

/**
 * @brief Alter the memory content.
 *
 * @param addr          An address.
 * @param new_bytes     New bytes.
 * @param origin_bytes  A buffer for storing original bytes. It's optional.
 *
 * @exception std::length_error The buffer for original bytes is too small.
 */
void AlterMemory(const std::intptr_t addr,
                 const std::span<const std::byte> new_bytes,
                 const std::span<std::byte> origin_bytes);

/**
 * @brief Format a long `jmp` instruction.
 *
 * @param from  The source address.
 * @param to    The destination address.
 * @return The machine code of the instruction.
 */
std::array<std::byte, JMP_LEN> FormatJmpBytes(const std::intptr_t from,
                                              const std::intptr_t to) noexcept;

/**
 * @brief Get the true entry address of a function.
 * @details Some compilers will generate a <em>Jump Thunk</em> for each function, using `&` operator can only get the address of the thunk.
 */
std::intptr_t GetFuncEntryAddr(const std::intptr_t addr) noexcept;

}  // namespace sys