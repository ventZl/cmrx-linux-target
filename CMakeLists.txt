cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_FLAGS_Debug "-ggdb3")

project(cmrx-on-linux)

set(CMRX_GDB_PATH /usr/bin/gdb)
set(CMRX_IDLE_THREAD_SHUTDOWN_CPU 1)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmrx/cmake)
add_compile_definitions(-DCMRX_VERBOSE_API_NAMES=1)

set(CMRX_ARCH linux)
set(CMRX_HAL posix)

include(CTest)

# Interface libraries so we can keep them empty
add_library(test_platform INTERFACE)
add_library(test_platform_main INTERFACE)

include(CMRX)

add_subdirectory(cmrx)
add_subdirectory(src/init)
add_subdirectory(src/fbdev)
add_subdirectory(src/contraption)
add_subdirectory(src/term)
add_subdirectory(src/util)
add_subdirectory(src/icons)
#add_subdirectory(src/presentation)

add_firmware(cmrx-on-linux src/main.c)
target_link_libraries(cmrx-on-linux cmrx)
target_add_applications(cmrx-on-linux init fbdev contraption term icons)
