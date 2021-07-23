add_library(zombie SHARED dllmain.cpp)
target_link_libraries(zombie PRIVATE game)