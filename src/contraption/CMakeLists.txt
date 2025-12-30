include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_application(contraption
    api.c
    server.c
    mouse.c
    gadget.c
    resources.c
    render.c
    window.c
    client.c
    gadgets/titlebar.c
    gadgets/button.c
    gadgets/menuitem.c
    gadgets/text.c
    gadgets/panel.c
    )

target_link_libraries(contraption stdlib)
