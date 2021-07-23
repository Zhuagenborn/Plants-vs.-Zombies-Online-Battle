#include "net_packet.h"
#include "hook.h"
#include "mod/interface.h"
#include "state.h"

#include <cassert>
#include <format>
#include <stdexcept>


namespace game::netpkg {

void Process(const Header* const packet) {
    assert(packet != nullptr);

    switch (packet->pkt_type) {
        case Type::NewPlant: {
            const NewItem* const item = static_cast<const NewItem*>(packet);
            mod::CreatePlant(item->pos_x, item->pos_y, item->id);
            break;
        }
        case Type::NewZombie: {
            const NewItem* const item = static_cast<const NewItem*>(packet);
            mod::CreateZombie(item->pos_x, item->pos_y, item->id);
            break;
        }
        case Type::LevelEnd: {
            mod::hook::LevelEnd{}.Disable();
            mod::EndLevel();
            break;
        }
        default: {
            throw std::invalid_argument{ "The type of packet is unknown." };
        }
    }
}


void RecvLoop(const std::stop_token stop_token) noexcept {
    try {
        while (!stop_token.stop_requested()) {
            net::Packet pkg{ net::Packet::Recv(*state::conn) };
            Process(reinterpret_cast<const Header*>(pkg.Read().data()));
        }
    } catch (const std::exception& err) {
        const auto msg{ std::format("Failed to receive or process a packet: {}",
                                    err.what()) };
        OutputDebugStringA(msg.c_str());
    }

    StopRecvLoop(false);
}


void StopRecvLoop(bool wait) noexcept {
    if (state::recv_thread.stop_src != nullptr) {
        state::recv_thread.stop_src->request_stop();
    }

    if (state::conn != nullptr) {
        state::conn->Close();
    }

    if (wait && state::recv_thread.thread != nullptr
        && state::recv_thread.thread->joinable()) {
        state::recv_thread.thread->join();
    }

    state::recv_thread.stop_src.reset();
}

}  // namespace game::netpkg