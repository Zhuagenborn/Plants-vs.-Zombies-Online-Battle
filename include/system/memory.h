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

//! The operation code of long @p jmp instruction.
constexpr std::byte jmp{ std::byte{ 0xE9 } };

//! The operation code of short @p jmp instruction.
constexpr std::byte short_jmp{ std::byte{ 0xEB } };

//! The operation code of @p call instruction.
constexpr std::byte call{ std::byte{ 0xE8 } };

//! The operation code of @p ret instruction.
constexpr std::byte ret{ std::byte{ 0xC3 } };

//! The operation code of @p nop instruction.
constexpr std::byte nop{ std::byte{ 0x90 } };

//! The length of long @p jmp instruction.
constexpr std::size_t jmp_len{ 5 };

//! The length of @p call instruction.
constexpr std::size_t call_len{ jmp_len };

/**
 * @brief Check if an address is @p nullptr.
 *
 * @param addr  An address.
 *
 * @exception std::invalid_argument The address is @p nullptr.
 */
void CheckNullPointer(std::intptr_t addr);

/**
 * @brief Make a part of memory writable.
 *
 * @param addr  A base address.
 * @param size  The size.
 *
 * @exception std::system_error The operation failed.
 */
void SetMemoryWritable(std::intptr_t addr, std::size_t size);

/**
 * @brief Alter the memory content.
 *
 * @param addr          An address.
 * @param new_bytes     New bytes.
 * @param origin_bytes  A buffer for storing original bytes. It's optional.
 *
 * @exception std::length_error The buffer for original bytes is too small.
 */
void AlterMemory(std::intptr_t addr, std::span<const std::byte> new_bytes,
                 std::span<std::byte> origin_bytes);

/**
 * @brief Format a long @p jmp instruction.
 *
 * @param from  The source address.
 * @param to    The destination address.
 * @return The machine code of the instruction.
 */
std::array<std::byte, jmp_len> FormatJmpBytes(std::intptr_t from,
                                              std::intptr_t to) noexcept;

/**
 * @brief Get the true entry address of a function.
 * @details Some compilers will generate a <em>Jump Thunk</em> for each function, using @p & operator can only get the address of the thunk.
 */
std::intptr_t GetFuncEntryAddr(std::intptr_t addr) noexcept;

}  // namespace sys