#include "memory.h"
#include "windows_error.h"

#include <cstring>
#include <stdexcept>


namespace sys {

void CheckNullPointer(const std::intptr_t addr) {
    if (addr == 0) {
        throw std::invalid_argument{ "The pointer is null." };
    }
}

void SetMemoryWritable(const std::intptr_t addr, const std::size_t size) {
    DWORD old_protect{ 0 };
    if (VirtualProtect(reinterpret_cast<void*>(addr), size,
                       PAGE_EXECUTE_READWRITE, &old_protect)
        == FALSE) {
        ThrowLastError();
    }
}


void AlterMemory(const std::intptr_t addr,
                 const std::span<const std::byte> new_bytes,
                 const std::span<std::byte> origin_bytes) {
    SetMemoryWritable(addr, new_bytes.size_bytes());

    if (!origin_bytes.empty()) {
        if (origin_bytes.size_bytes() < new_bytes.size_bytes()) {
            throw std::length_error{
                "The buffer for original bytes is too small."
            };
        }

        std::memcpy(origin_bytes.data(), reinterpret_cast<void*>(addr),
                    new_bytes.size_bytes());
    }

    std::memcpy(reinterpret_cast<void*>(addr), new_bytes.data(),
                new_bytes.size_bytes());
}


std::array<std::byte, JMP_LEN> FormatJmpBytes(const std::intptr_t from,
                                              const std::intptr_t to) noexcept {
    std::array<std::byte, JMP_LEN> bytes{ JMP };
    const auto offset{ to - from - JMP_LEN };
    std::memcpy(bytes.data() + 1, &offset, sizeof(offset));
    return bytes;
}


std::intptr_t GetFuncEntryAddr(const std::intptr_t addr) noexcept {
    const auto op{ *reinterpret_cast<std::byte*>(addr) };

    if (op != JMP) {
        return addr;
    } else {
        const auto offset{ *reinterpret_cast<std::ptrdiff_t*>(
            addr + sizeof(std::byte)) };
        return addr + offset + JMP_LEN;
    }
}

}  // namespace sys