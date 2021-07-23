#include "mod.h"

#include "memory.h"

#include <array>
#include <cassert>
#include <fstream>
#include <memory>
#include <stdexcept>


namespace game::mod {

using namespace sys;

SetSunAmount::SetSunAmount(const std::size_t amount) noexcept :
    amount_{ amount } {}

std::string_view SetSunAmount::Name() const noexcept {
    return "Set-Sun-Amount";
}

void SetSunAmount::Enable() {
    const auto addr1{ *reinterpret_cast<std::intptr_t*>(BASE_ADDR) };
    if (addr1 == 0) {
        return;
    }

    const auto addr2{ *reinterpret_cast<std::intptr_t*>(addr1 + 0x768) };
    if (addr2 == 0) {
        return;
    }

    *reinterpret_cast<std::size_t*>(addr2 + 0x5560) = amount_;
}


std::string_view DisableAutoPause::Name() const noexcept {
    return "Disable-Auto-Pause";
}

void DisableAutoPause::Enable() {
    AlterMemory(0x0044F478, std::array{ RET, NOP, NOP, NOP, NOP }, {});
}

std::string_view RemoveDefaultPlants::Name() const noexcept {
    return "Remove-Default-Plants";
}

void RemoveDefaultPlants::Enable() {
    const auto addr1{ *reinterpret_cast<std::intptr_t*>(BASE_ADDR) };
    if (addr1 == 0) {
        return;
    }

    const auto addr2{ *reinterpret_cast<std::intptr_t*>(addr1 + 0x768) };
    if (addr2 == 0) {
        return;
    }

    const auto plant_count{ *reinterpret_cast<std::size_t*>(addr2 + 0xB0) };
    const auto begin_addr{ *reinterpret_cast<std::intptr_t*>(addr2 + 0xAC) };
    const auto end_addr{ begin_addr + sizeof(PlantedPlant) * plant_count };

    for (auto curr_addr{ begin_addr }; curr_addr != end_addr;
         curr_addr += sizeof(PlantedPlant)) {
        reinterpret_cast<PlantedPlant*>(curr_addr)->invalid = true;
    }
}


std::string_view AllowMultiProcess::Name() const noexcept {
    return "Allow-Multi-Process";
}

void AllowMultiProcess::Enable() {
    AlterMemory(VIRTUAL_ADDR, std::array{ SHORT_JMP }, {});
}

void AllowMultiProcess::Enable(const std::string_view file_path) {
    if (file_path.empty()) {
        throw std::invalid_argument{ "The process path is empty." };
    }

    std::fstream file{};

    auto deleter{ [](std::fstream* const file) {
        assert(file != nullptr);
        file->close();
    } };

    std::unique_ptr<std::fstream, decltype(deleter)> raii{ &file, deleter };

    file.open(file_path.data(),
              std::ios::in | std::ios::out | std::ios::binary);
    file.seekp(RAW_OFFSET);
    file.write(reinterpret_cast<const char*>(&SHORT_JMP), sizeof(SHORT_JMP));
}

}  // namespace game::mod