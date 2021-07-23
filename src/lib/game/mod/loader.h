/**
 * @file loader.h
 * @brief The modification loader.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-07-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#pragma once

#include "interface.h"

#include <list>
#include <memory>


namespace game::mod {

/**
 * @brief The modification loader.
 */
class Loader final {
public:
    /**
     * @brief Register a modification.
     *
     * @param mod   A modification.
     * @return The current loader.
     */
    Loader& Add(std::unique_ptr<Mod> mod);

    //! Load registered modifications.
    void Load();

private:
    /**
     * @brief Check whether a modification has been registered twice.
     *
     * @param mod   A modification.
     *
     * @exception std::invalid_argument The modification has been registered before.
     */
    void CheckDuplicate(const Mod& mod) const;

    std::list<std::unique_ptr<Mod>> mods_{};
};

}  // namespace game::mod