cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_FLAGS_Debug "-ggdb3")

project(cmrx-on-linux)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmrx/cmake)
add_compile_definitions(-DCMRX_VERBOSE_API_NAMES=1)

set(CMRX_ARCH linux)
set(CMRX_HAL posix)

include(CMRX)

add_subdirectory(cmrx)
add_subdirectory(src/init)
add_subdirectory(src/fbdev)
add_subdirectory(src/contraption)

add_firmware(cmrx-on-linux src/main.c)
target_link_libraries(cmrx-on-linux cmrx)
target_add_applications(cmrx-on-linux init fbdev contraption)
