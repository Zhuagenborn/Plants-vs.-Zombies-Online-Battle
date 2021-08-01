add_library(zombie SHARED dllmain.cpp)
set_target_properties(zombie PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
target_link_libraries(zombie PRIVATE game)