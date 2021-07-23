#include "loader.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>
#include <utility>


namespace game::mod {

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

}  // namespace game::mod