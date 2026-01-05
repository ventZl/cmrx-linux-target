include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_application(contraption
    api.c
    server.c
    mouse.c
    keyboard.c
    gadget.c
    resources.c
    render.c
    window.c
    client.c
    desktop.c
    dock.c
    dock_win.c
    gadgets/titlebar.c
    gadgets/button.c
    gadgets/menuitem.c
    gadgets/text.c
    gadgets/panel.c
    gadgets/pixmap.c
    pixmap.c
    )

target_link_libraries(contraption stdlib)
