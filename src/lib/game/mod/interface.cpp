#include "interface.h"


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

}  // namespace mod

}  // namespace game