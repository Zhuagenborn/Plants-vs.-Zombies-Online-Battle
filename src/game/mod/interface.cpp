#include "interface.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>
#include <utility>


namespace game {

Mod::~Mod() noexcept = default;

void Mod::Disable(){};


namespace mod {

__declspec(naked) void CreateZombie(const std::int32_t pos_x,
                                    const std::int32_t pos_y,
                                    const std::int32_t id) noexcept {
    __asm {
        pushad

        mov     ebx, dword ptr ds : [BASE_ADDR]
        test    ebx, ebx
        jz      _end
        mov     ebx, dword ptr ds : [ebx + 0x768]
        test    ebx, ebx
        jz      _end

        mov     ecx, dword ptr ds : [ebx + 0x160]

        mov     edx, [esp + 32 + 4]
        push    edx

        mov     eax, [esp + 32 + 16]
        push    eax

        mov     eax, [esp + 32 + 16]

        mov     edx, FUNC_CREATE_ZOMBIE_ADDR
        call    edx

    _end:
        popad
        ret
    }
}

__declspec(naked) void CreatePlant(const std::int32_t pos_x,
                                   const std::int32_t pos_y,
                                   const std::int32_t id) noexcept {
    __asm {
        pushad

        push    -1

        mov     eax, [esp + 32 + 16]
        push    eax

        mov     edx, [esp + 32 + 12]
        push    edx

        mov     ebx, dword ptr ds : [BASE_ADDR]
        test    ebx, ebx
        jz      _end

        mov     ebx, dword ptr ds : [ebx + 0x768]
        push    ebx

        mov     eax, [esp + 32 + 24]

        mov     edx, FUNC_CREATE_PLANT_ADDR
        call    edx
        popad
        ret

    _end:
        add     esp, 12
        popad
        ret
    }
}

__declspec(naked) void EndLevel() noexcept {
    __asm {
        pushad

        mov     ebx, dword ptr ds : [BASE_ADDR]
        test    ebx, ebx
        jz      _end
        mov     eax, dword ptr ds : [ebx + 0x768]
        test    eax, eax
        jz      _end

        push    0
        push    eax
        mov     edx, FUNC_END_LEVEL_ADDR
        call    edx

    _end:
        popad
        ret
    }
}


Loader& Loader::Add(std::unique_ptr<Mod> mod) {
    if (mod == nullptr) {
        throw std::invalid_argument{ "The module is null." };
    }

    CheckDuplicate(*mod);
    mods_.push_back(std::move(mod));
    return *this;
}


void Loader::Load() {
    std::ranges::for_each(mods_, [](auto& mod) { mod->Enable(); });
}

void Loader::CheckDuplicate(const Mod& mod) const {
    auto cmp{ [&mod](const auto& curr) {
        assert(curr != nullptr);
        return mod.Name().compare(curr->Name()) == 0;
    } };

    if (const auto it{ std::ranges::find_if(mods_, cmp) }; it != mods_.cend()) {
        throw std::invalid_argument{ std::format(
            "The module '{}' has been registered.", mod.Name()) };
    }
}

}  // namespace mod

}  // namespace game