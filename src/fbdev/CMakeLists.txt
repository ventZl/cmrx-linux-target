include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_application(fbdev
    fbdev.c
    fbdev_api.c
    renderer/text.c
    fonts/NotoSans14.c
    fonts/NotoSans14b.c
    fonts/NotoSans14bi.c
    fonts/NotoSans14i.c
    fonts/HackNerdMono14.c
    fonts/HackNerdMono14b.c
    fonts/HackNerdMono14bi.c
    fonts/HackNerdMono14i.c
    fonts/HackNerdMono32b.c
    fonts/HackNerdMono64b.c
    fonts/Roboto32.c
    fonts/Roboto48b.c
    fonts/RobotoBlack64b.c
    pointer.c
    keyboard.c
    )
target_link_libraries(fbdev SDL3::SDL3 stdlib)


