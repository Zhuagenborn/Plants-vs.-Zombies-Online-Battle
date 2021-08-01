add_library(game "")
# target_compile_definitions(game PRIVATE INET6=1)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/game)
target_include_directories(game PUBLIC ${HEADER_PATH})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_sources(game
    PUBLIC
        ${HEADER_PATH}/config.h
        ${HEADER_PATH}/startup.h
    PRIVATE
        state.h
        state.cpp
        startup.cpp
        config.cpp

        mod/interface.h
        mod/interface.cpp
        mod/mod.h
        mod/mod.cpp
        mod/loader.h
        mod/loader.cpp

        mod/hook/interface.h
        mod/hook/interface.cpp
        mod/hook/hook.h
        mod/hook/hook.cpp
        mod/hook/net_packet.h
        mod/hook/net_packet.cpp
)

target_link_libraries(game PUBLIC network)
target_link_libraries(game PRIVATE system)