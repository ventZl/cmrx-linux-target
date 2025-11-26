include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_application(fbdev
    fbdev.c
    fbdev_api.c
    renderer/text.c
    fonts/NotoSans14.c
    fonts/NotoSans14b.c
    pointer.c)
target_link_libraries(fbdev SDL3::SDL3 stdlib)


