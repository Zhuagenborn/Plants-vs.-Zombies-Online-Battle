/**
 * @file main_page.h
 * @brief The main Doxygen page.
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

/**
 * @mainpage Plants vs. Zombies Online Battle
 *
 * @section introduction Introduction
 *
 * This project adds the multiplayer battle to ***Plants vs. Zombies*** via reverse engineering, inline hook and dynamic-link library injection. Two online players can defend and attack as *Plant* and *Zombie* respectively.
 *
 * @section getting-started Getting Started
 *
 * @subsection prerequisites Prerequisites
 *
 * - Install [*Visual Studio 2022*](https://visualstudio.microsoft.com).
 * - Install [*CMake*](https://cmake.org).
 * - Set the `PATH` environment variables.
 *
 * @subsection building Building
 *
 * Set the location to the project folder and run:
 *
 * ```bash
 * mkdir -p build
 * cd build
 * cmake .. -G "Visual Studio 17 2022" -A Win32
 * cmake --build .
 * ```
 *
 * Two dynamic-link libraries `plant.dll` and `zombie.dll` will be generated in `build/bin` folder. Copy them to the game root folder.
 *
 * @subsubsection ipv6 IPv6
 *
 * The default *IP* version is *IPv4*. Enable the following statement in `libs/game/CMakeLists.txt` if you want to build *IPv6* libraries.
 *
 * ```cmake
 * target_compile_definitions(game PRIVATE INET6=1)
 * ```
 *
 * @section usage Usage
 *
 * > The project only works with *Plants vs. Zombies **1.0.0.1051 CHINESE*** version, provided in `game` folder.
 * >
 * > The *MD5* of `PlantsVsZombies.exe` is `37B729B4056131722A556E646AC915E9`.
 *
 * In order to activate online functions, `plant.dll` and `zombie.dll` must be injected into the game when it starts. You can directly use this simple injection tool: [*Dll-Injector*](https://github.com/Zhuagenborn/Dll-Injector).
 *
 * @subsection playing-as-plant Playing as Plant
 *
 * If a player plays as the plant, the game will launch as a server.
 *
 * ```bash
 * Dll-Injector -f PlantsVsZombies.exe plant.dll
 * ```
 *
 * Or use `Start-OnlineGame.ps1` directly. It needs to be copied to the game root folder.
 *
 * ```powershell
 * Start-OnlineGame.ps1 -Role Plant
 * ```
 *
 * Start *I, Zombie Endless* level, the game will pause and wait for a client to connect.
 *
 * @subsection playing-as-zombie Playing as Zombie
 *
 * If a player plays as the zombie, the game will launch as a client.
 *
 * ```bash
 * Dll-Injector -f PlantsVsZombies.exe zombie.dll
 * ```
 *
 * Or use `Start-OnlineGame.ps1`.
 *
 * ```powershell
 * Start-OnlineGame.ps1 -Role Zombie
 * ```
 *
 * Start *I, Zombie Endless* level, the game will try to connect to the server.
 *
 * @subsection i-zombie-ndless I, Zombie Endless
 *
 * If the modification has been loaded successfully, ***I, Zombie Endless*** level will be converted into an online level. If your current progress does not have this level, you can copy `game/userdata` to `C:\ProgramData\PopCap Games\PlantsVsZombies\userdata`. Remember to back up your own save-files before copying.
 *
 * @section configurations Configurations
 *
 * Copy `online_config.ini` to the game root folder. You can set the server's IP address and port number in it.
 *
 * ```ini
 * [Network]
 * ServerIP=127.0.0.1
 * Port=10000
 * ```
 *
 * @section documents Documents
 *
 * The code comment style follows the [*Doxygen*](http://www.doxygen.nl) specification.
 *
 * `docs/Key Data and Functions.md` describes key data and functions obtained by reverse engineering.
 *
 * @section issues-bugs Issues & Bugs
 *
 * - The game sometimes crashes when creating zombies.
 * - If the progress of *I, Zombie Endless* level is not empty, two players will have different battlefields.
 * - In order to simplify the code, the running menu and automatic pause are disabled.
 *
 * @section license License
 *
 * Distributed under the *MIT License*. See `LICENSE` for more information.
 */