add_library(system "")
set_target_properties(system PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

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