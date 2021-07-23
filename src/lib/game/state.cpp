#include "state.h"


namespace game::state {

Role role{};

Config cfg{};

std::unique_ptr<net::TcpSocket<cfg::IpAddr>> conn{};

StoppableThread recv_thread{};

}  // namespace game::state