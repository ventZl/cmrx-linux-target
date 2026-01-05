#pragma once

struct CWindow;

int menus_open(struct CWindow * top_menu, struct CWindow * menus[]);
void menus_close(int top_menu_id, struct CWindow * top_menu);
