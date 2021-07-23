add_library(system "")

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/system)
target_include_directories(system PUBLIC ${HEADER_PATH})

target_sources(system
    PUBLIC
        ${HEADER_PATH}/windows_error.h
        ${HEADER_PATH}/memory.h
    PRIVATE
        windows_error.cpp
        memory.cpp
)