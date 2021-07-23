#include "interface.h"

#include "memory.h"

#include <cstring>


namespace game {

using namespace sys;

void Hook::Enable() {
    const auto from{ From() };
    Trampoline trampoline{ HookBytes() };
    std::vector hook_bytes{ trampoline.code };
    const auto offset{ To() - from - trampoline.jmp_inst_len };

    std::memcpy(hook_bytes.data() + trampoline.jmp_offset_pos, &offset,
                sizeof(offset));

    origin_bytes_.resize(hook_bytes.size());
    AlterMemory(from, hook_bytes, origin_bytes_);

    const std::optional jump_ret{ JumpRet() };
    if (jump_ret.has_value()) {
        const std::array ret_bytes{ FormatJmpBytes(jump_ret.value(),
                                                   from + hook_bytes.size()) };
        AlterMemory(jump_ret.value(), ret_bytes, {});
    }
}


void Hook::Disable() {
    AlterMemory(From(), origin_bytes_, {});
}

std::optional<std::intptr_t> Hook::JumpRet() const noexcept {
    return std::nullopt;
}

std::span<const std::byte> Hook::OriginBytes() const noexcept {
    return origin_bytes_;
}

}  // namespace game