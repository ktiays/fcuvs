cmake_minimum_required(VERSION 3.27)
project(fcuvs VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

find_library(OBJC_LIBRARY objc)
find_library(FOUNDATION_LIBRARY Foundation)
find_library(APPKIT_LIBRARY AppKit)

add_executable(fcuvs src/main.cc)

target_link_libraries(fcuvs ${OBJC_LIBRARY} ${FOUNDATION_LIBRARY} ${APPKIT_LIBRARY})
target_include_directories(fcuvs PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(fcuvs 
    PRIVATE 
    $<$<CONFIG:Release>:-O3 -march=native -flto -ffast-math -g0>
)
target_link_options(fcuvs 
    PRIVATE 
    $<$<CONFIG:Release>:-Wl,-dead_strip>
)