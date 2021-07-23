#include "hook.h"
#include "mod/loader.h"
#include "mod/mod.h"
#include "net_packet.h"
#include "state.h"

#include "listener.h"
#include "memory.h"
#include "packet.h"
#include "socket/tcp.h"

#include <cassert>
#include <exception>
#include <format>
#include <thread>
#include <typeinfo>
#include <utility>


namespace game::mod::hook {

using namespace sys;

Hook::Trampoline BeforeLoadLevel::HookBytes() const noexcept {
    return { .code{ CALL, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP },
             .jmp_offset_pos{ sizeof(CALL) },
             .jmp_inst_len{ CALL_LEN } };
}


std::string_view BeforeLoadLevel::Name() const noexcept {
    return "Hook-Before-Load-Level";
}

std::intptr_t BeforeLoadLevel::From() const noexcept {
    return 0x0044F560;
}

std::intptr_t BeforeLoadLevel::To() const noexcept {
    return reinterpret_cast<std::intptr_t>(&Detour);
}

__declspec(naked) void __stdcall BeforeLoadLevel::Detour() noexcept {
    __asm {
        cmp     dword ptr ds : [esp + 8], MOD_LEVEL_ID
        jnz     _end
        cmp     dword ptr ds : [esp + 12], 1
        jnz     _end

        pushad
        mov     eax, Callback
        call    eax
        popad

    _end:
        mov     eax, dword ptr fs : [0]
        ret
    }
}


void __stdcall BeforeLoadLevel::Callback() noexcept {
    netpkg::StopRecvLoop(true);

    Loader loader{};
    loader.Add(std::make_unique<DisableAutoPause>())
        .Add(std::make_unique<DisableRuntimeMenu>())
        .Add(std::make_unique<LevelEnd>())
        .Add(std::make_unique<CreateZombie>());

    if (state::role == Role::Plant) {
        loader.Add(std::make_unique<CreatePlant>());
    }

    try {
        loader.Load();

        if (state::role == Role::Plant) {
            net::Listener<cfg::IpAddr> listener{};

            std::string_view ip{};
            if (typeid(cfg::IpAddr) == typeid(net::Ipv4Addr)) {
                ip = net::Ipv4Addr::LOOP_BACK;
            } else if (typeid(cfg::IpAddr) == typeid(net::Ipv6Addr)) {
                ip = net::Ipv6Addr::LOOP_BACK;
            } else {
                assert(false);
                std::abort();
            }

            listener.Bind(cfg::IpAddr{ ip, state::cfg.Network().Port() });
            listener.Listen();
            state::conn = std::make_unique<net::TcpSocket<cfg::IpAddr>>(
                listener.Accept());

        } else if (state::role == Role::Zombie) {
            state::conn = std::make_unique<net::TcpSocket<cfg::IpAddr>>();
            state::conn->Connect(cfg::IpAddr{ state::cfg.Network().ServerIp(),
                                              state::cfg.Network().Port() });
        } else {
            assert(false);
            std::abort();
        }

    } catch (const std::exception& err) {
        state::conn.reset();
        const auto msg{ std::format("Failed to start an online battle: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
        MessageBoxA(nullptr, msg.c_str(), "Online Battle", MB_ICONERROR);
        return;
    }

    state::recv_thread.stop_src = std::make_unique<std::stop_source>();
    state::recv_thread.thread = std::make_unique<std::jthread>(
        netpkg::RecvLoop, state::recv_thread.stop_src->get_token());
}


Hook::Trampoline AfterLoadLevel::HookBytes() const noexcept {
    return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP },
             .jmp_offset_pos{ sizeof(JMP) },
             .jmp_inst_len{ JMP_LEN } };
}


std::string_view AfterLoadLevel::Name() const noexcept {
    return "Hook-After-Load-Level";
}

std::intptr_t AfterLoadLevel::From() const noexcept {
    return 0x0042F7BC;
}

std::intptr_t AfterLoadLevel::To() const noexcept {
    return reinterpret_cast<std::intptr_t>(Detour);
}

std::optional<std::intptr_t> AfterLoadLevel::JumpRet() const noexcept {
    return GetFuncEntryAddr(To()) + DETOUR_LEN - JMP_LEN;
}

void __stdcall AfterLoadLevel::Callback() noexcept {
    try {
        Loader{}
            .Add(std::make_unique<SetSunAmount>(10000))
            .Add(std::make_unique<RemoveDefaultPlants>())
            .Add(std::make_unique<InitSlots>())
            .Load();
    } catch (const std::exception& err) {
        netpkg::StopRecvLoop(true);
        const auto msg{ std::format("Failed to start an online battle: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
        MessageBoxA(nullptr, msg.c_str(), "Online Battle", MB_ICONERROR);
    }
}


__declspec(naked) void __stdcall AfterLoadLevel::Detour() noexcept {
    __asm {
        cmp     dword ptr ds : [esp - 8], 0x46
        jnz     _end
        cmp     dword ptr ds : [esp - 4], 1
        jnz     _end

        pushad
        mov     eax, Callback
        call    eax
        popad

    _end:
        lea     eax, [ebp - 0x12C]
        // jmp
        nop
        nop
        nop
        nop
        nop
    }
}


Hook::Trampoline DisableRuntimeMenu::HookBytes() const noexcept {
    return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP },
             .jmp_offset_pos{ sizeof(JMP) },
             .jmp_inst_len{ JMP_LEN } };
}


std::string_view DisableRuntimeMenu::Name() const noexcept {
    return "Disable-Runtime-Menu";
}

std::intptr_t DisableRuntimeMenu::From() const noexcept {
    return 0x00450102;
}

std::intptr_t DisableRuntimeMenu::To() const noexcept {
    return 0x0045016A;
}


bool InitSlots::initialized_{ false };

InitSlots::InitSlots() noexcept {
    initialized_ = false;
}

Hook::Trampoline InitSlots::HookBytes() const noexcept {
    return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP, NOP },
             .jmp_offset_pos{ sizeof(JMP) },
             .jmp_inst_len{ JMP_LEN } };
}


std::string_view InitSlots::Name() const noexcept {
    return "Hook-Initialize-Slots";
}

std::intptr_t InitSlots::From() const noexcept {
    return 0x00488220;
}

std::intptr_t InitSlots::To() const noexcept {
    return reinterpret_cast<std::intptr_t>(Detour);
}

std::optional<std::intptr_t> InitSlots::JumpRet() const noexcept {
    return GetFuncEntryAddr(To()) + DETOUR_LEN - JMP_LEN;
}

__declspec(naked) void __stdcall InitSlots::Detour() noexcept {
    __asm {
        pushad
        push    dword ptr ss : [esp + 32 + 4]
        mov     edx, SetSlot
        call    edx
        popad
        push    -1
        push    0x0064EA08
            // jmp
        nop
        nop
        nop
        nop
        nop
    }
}


void __stdcall InitSlots::SetSlot(Slot& slot) noexcept {
    if (initialized_) {
        return;
    }

    static std::int32_t i{ 0 };
    if (state::role == Role::Plant) {
        slot.id = state::cfg.Player().PlantSlots()[i];
    } else if (state::role == Role::Zombie) {
        slot.id = state::cfg.Player().ZombieSlots()[i] + ZOMBIE_ID_BASE;
    } else {
        assert(false);
        std::abort();
    }

    if (++i == SLOT_NUM) {
        initialized_ = true;
        i = 0;
    }
}


Hook::Trampoline LevelEnd::HookBytes() const noexcept {
    return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP },
             .jmp_offset_pos{ sizeof(JMP) },
             .jmp_inst_len{ JMP_LEN } };
}


std::string_view LevelEnd::Name() const noexcept {
    return "Hook-Level-End";
}

std::intptr_t LevelEnd::From() const noexcept {
    return FUNC_END_LEVEL_ADDR;
}

std::intptr_t LevelEnd::To() const noexcept {
    return reinterpret_cast<std::intptr_t>(Detour);
}

std::optional<std::intptr_t> LevelEnd::JumpRet() const noexcept {
    return GetFuncEntryAddr(To()) + DETOUR_LEN - JMP_LEN;
}

void __stdcall LevelEnd::Callback() noexcept {
    if (state::conn == nullptr || !state::conn->Valid()) {
        return;
    }

    netpkg::Header lvl_end{};
    lvl_end.size = sizeof(lvl_end);
    lvl_end.pkt_type = netpkg::Type::LevelEnd;
    lvl_end.role = state::role;

    try {
        net::Packet pkg{};
        pkg.Write({ reinterpret_cast<std::byte*>(&lvl_end), sizeof(lvl_end) });
        pkg.Send(*state::conn);

        netpkg::StopRecvLoop(true);

    } catch (const std::exception& err) {
        const auto msg{ std::format("Failed to send a packet: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
    }
}


__declspec(naked) void __stdcall LevelEnd::Detour() noexcept {
    __asm {
        pushad
        mov     eax, Callback
        call    eax
        popad
        push    ebp
        mov     ebp, esp
        and     esp, 0xFFFFFFF8
        // jmp
        nop
        nop
        nop
        nop
        nop
    }
}


Hook::Trampoline CreateZombie::HookBytes() const noexcept {
    if (state::role == Role::Plant) {
        return { .code{ CALL, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                        std::byte{ 0 } },
                 .jmp_offset_pos{ sizeof(CALL) },
                 .jmp_inst_len{ CALL_LEN } };
    } else if (state::role == Role::Zombie) {
        return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                        std::byte{ 0 }, NOP },
                 .jmp_offset_pos{ sizeof(JMP) },
                 .jmp_inst_len{ JMP_LEN } };
    } else {
        assert(false);
        std::abort();
    }
}


std::string_view CreateZombie::Name() const noexcept {
    return "Hook-Create-Zombie";
}

std::intptr_t CreateZombie::From() const noexcept {
    if (state::role == Role::Plant) {
        return 0x0042A425;
    } else if (state::role == Role::Zombie) {
        return FUNC_CREATE_ZOMBIE_ADDR;
    } else {
        assert(false);
        std::abort();
    }
}


std::intptr_t CreateZombie::To() const noexcept {
    if (state::role == Role::Plant) {
        return reinterpret_cast<std::intptr_t>(PlantDetour);
    } else if (state::role == Role::Zombie) {
        return reinterpret_cast<std::intptr_t>(ZombieDetour);
    } else {
        assert(false);
        std::abort();
    }
}


std::optional<std::intptr_t> CreateZombie::JumpRet() const noexcept {
    if (state::role == Role::Plant) {
        return std::nullopt;
    } else if (state::role == Role::Zombie) {
        return GetFuncEntryAddr(To()) + ZOMBIE_DETOUR_LEN - JMP_LEN;
    } else {
        assert(false);
        std::abort();
    }
}


void __stdcall CreateZombie::ZombieCallback(const std::int32_t pos_x,
                                            const std::int32_t pos_y,
                                            const std::int32_t id) noexcept {
    if (state::conn == nullptr || !state::conn->Valid()) {
        return;
    }

    netpkg::NewItem new_item{};
    new_item.size = sizeof(new_item);
    new_item.pkt_type = netpkg::Type::NewZombie;
    new_item.role = Role::Zombie;
    new_item.pos_x = pos_x;
    new_item.pos_y = pos_y;
    new_item.id = id;

    try {
        net::Packet pkg{};
        pkg.Write(
            { reinterpret_cast<std::byte*>(&new_item), sizeof(new_item) });
        pkg.Send(*state::conn);

    } catch (const std::exception& err) {
        const auto msg{ std::format("Failed to send a packet: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
    }
}


__declspec(naked) void __stdcall CreateZombie::ZombieDetour() noexcept {
    __asm {
        pushad
        push    dword ptr ss : [esp + 32 + 4]
        push    eax
        push    dword ptr ss : [esp + 32 + 16]
        mov     eax, ZombieCallback
        call    eax
        popad

        push    ebx
        push    ebp
        mov     ebp, dword ptr ss : [esp + 0xC]
        // jmp
        nop
        nop
        nop
        nop
        nop
    }
}


__declspec(naked) void __stdcall CreateZombie::PlantDetour() noexcept {
    __asm {
        pushad
        mov     edx, dword ptr ss : [esp + 32 + 4]
        mov     ebx, dword ptr ss : [esp + 32 + 8]
        push    -1
        push    edx
        push    ebx
        mov     ebx, dword ptr ds : [BASE_ADDR]
        mov     ebx, dword ptr ds : [ebx + 0x768]
        push    ebx
        mov     edx, FUNC_CREATE_PLANT_ADDR
        call    edx
        popad
        retn    8
    }
}


Hook::Trampoline CreatePlant::HookBytes() const noexcept {
    return { .code{ JMP, std::byte{ 0 }, std::byte{ 0 }, std::byte{ 0 },
                    std::byte{ 0 }, NOP, NOP },
             .jmp_offset_pos{ sizeof(JMP) },
             .jmp_inst_len{ JMP_LEN } };
}


std::string_view CreatePlant::Name() const noexcept {
    return "Hook-Create-Plant";
}

std::intptr_t CreatePlant::From() const noexcept {
    return FUNC_CREATE_PLANT_ADDR;
}

std::intptr_t CreatePlant::To() const noexcept {
    return reinterpret_cast<std::intptr_t>(Detour);
}

std::optional<std::intptr_t> CreatePlant::JumpRet() const noexcept {
    return GetFuncEntryAddr(To()) + DETOUR_LEN - JMP_LEN;
}

void __stdcall CreatePlant::Callback(const std::int32_t pos_x,
                                     const std::int32_t pos_y,
                                     const std::int32_t id) noexcept {
    if (state::conn == nullptr || !state::conn->Valid()) {
        return;
    }

    netpkg::NewItem new_item{};
    new_item.size = sizeof(new_item);
    new_item.pkt_type = netpkg::Type::NewPlant;
    new_item.role = Role::Plant;
    new_item.pos_x = pos_x;
    new_item.pos_y = pos_y;
    new_item.id = id;

    try {
        net::Packet pkg{};
        pkg.Write(
            { reinterpret_cast<std::byte*>(&new_item), sizeof(new_item) });
        pkg.Send(*state::conn);
    } catch (const std::exception& err) {
        const auto msg{ std::format("Failed to send a packet: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
    }
}


__declspec(naked) void __stdcall CreatePlant::Detour() noexcept {
    __asm {
        pushad
        push    dword ptr ss : [esp + 32 + 12]
        push    eax
        push    dword ptr ss : [esp + 32 + 16]
        mov     eax, Callback
        call    eax
        popad

        push    ecx
        push    ebx
        push    ebp
        mov     ebp, dword ptr ss : [esp + 0x10]
        // jmp
        nop
        nop
        nop
        nop
        nop
    }
}

}  // namespace game::mod::hook