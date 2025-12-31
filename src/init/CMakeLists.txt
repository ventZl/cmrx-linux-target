add_application(init init.c server.c fuzzer.c file_dlg.c about_dlg.c init_menu.c navigator_win.c preferences_dlg.c)
target_link_libraries(init stdlib)

