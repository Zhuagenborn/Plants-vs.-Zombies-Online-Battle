add_library(plant SHARED dllmain.cpp)
target_link_libraries(plant PRIVATE game)