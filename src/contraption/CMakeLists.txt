include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_application(contraption
    api.c
    server.c
    mouse.c
    gadget.c
    resources.c
    gadgets/titlebar.c
    gadgets/button.c
    gadgets/menuitem.c
    gadgets/text.c
    )

target_link_libraries(contraption stdlib)
