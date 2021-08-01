add_library(plant SHARED dllmain.cpp)
set_target_properties(plant PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
target_link_libraries(plant PRIVATE game)