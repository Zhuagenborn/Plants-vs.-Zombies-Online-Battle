#include "config.h"

#include <Windows.h>


namespace game {

namespace cfg {

const std::map<std::string_view, std::int32_t> plant_ids{};

const std::map<std::string_view, std::int32_t> zombie_ids{};

const Slots Player::default_zombies{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

const Slots Player::default_plants{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

const Slots& Player::ZombieSlots() const noexcept {
    return zombies_;
}

const Slots& Player::PlantSlots() const noexcept {
    return plants_;
}


Network::Network() noexcept = default;

Network::Network(const std::string_view file) noexcept {
    char ip[256]{};
    if (const auto ip_size{ GetPrivateProfileStringA(INI_SECTION.data(),
                                                     IP_INI_KEY.data(), "", ip,
                                                     sizeof(ip), file.data()) };
        ip_size != 0) {
        server_ip_ = ip;
    }

    port_ = GetPrivateProfileIntA(INI_SECTION.data(), PORT_INI_KEY.data(),
                                  default_port, file.data());
}


std::string_view Network::ServerIp() const noexcept {
    return server_ip_;
}

std::uint16_t Network::Port() const noexcept {
    return port_;
}

}  // namespace cfg


Config::Config() noexcept = default;

Config::Config(const std::string_view file) noexcept : network_{ file } {}

const cfg::Player& Config::Player() const noexcept {
    return player_;
}

const cfg::Network& Config::Network() const noexcept {
    return network_;
}

}  // namespace game