#include "startup.h"
#include "mod/hook/hook.h"
#include "mod/loader.h"
#include "mod/mod.h"
#include "state.h"

#include <utility>


namespace game {

Startup::Startup(const Role role, Config cfg) noexcept {
    state::cfg = std::move(cfg);
    state::role = role;
}

Startup::~Startup() noexcept {
    Stop();
}

void Startup::Run() {
    mod::Loader{}
        .Add(std::make_unique<mod::AllowMultiProcess>())
        .Add(std::make_unique<mod::hook::BeforeLoadLevel>())
        .Add(std::make_unique<mod::hook::AfterLoadLevel>())
        .Load();
}


void Startup::Stop() noexcept {
    state::conn.reset();
}

}  // namespace game