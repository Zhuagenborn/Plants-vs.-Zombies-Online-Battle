add_library(network "")

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(network PUBLIC ${HEADER_PATH})

target_sources(network
    PUBLIC
        ${HEADER_PATH}/ip_addr.h
        ${HEADER_PATH}/packet.h
    INTERFACE
        ${HEADER_PATH}/listener.h
        ${HEADER_PATH}/socket/tcp.h
    PRIVATE
        ${HEADER_PATH}/socket/basic.h
        socket/basic.cpp
        ip_addr.cpp
        packet.cpp
)

target_link_libraries(network PUBLIC system)