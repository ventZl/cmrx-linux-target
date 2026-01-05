#include "../contraption/contraption.h"
#include "events.h"

struct CGadget init_menu_items[] = {
    GADGET_MENUWINDOW_ITEM(0, 90, "About Init", EVENT_INIT_ABOUT, GADGET_MENU_ITEM_ENABLED),
    GADGET_MENUWINDOW_ITEM(1, 90, "Preferences...", EVENT_INIT_ABOUT, GADGET_MENU_ITEM_ENABLED),
};

struct CWindow init_menu = {
    .top = 20,
    .left = 0,
    .width = 90,
    .height = 40,
    .gadgets = init_menu_items,
    .gadget_count = GADGET_COUNT(init_menu_items),
    .background = BACKGROUND_WHITE
};


struct CGadget file_menu_items[] = {
    GADGET_MENUWINDOW_ITEM(0, 90, "New", EVENT_FILE_NEW, GADGET_MENU_ITEM_ENABLED),
    GADGET_MENUWINDOW_ITEM(1, 90, "Open...", EVENT_FILE_OPEN, GADGET_MENU_ITEM_ENABLED),
    GADGET_MENUWINDOW_ITEM(2, 90, "Save", EVENT_FILE_SAVE, GADGET_MENU_ITEM_DISABLED),
    GADGET_MENUWINDOW_ITEM(3, 90, "Quit", EVENT_FILE_QUIT, GADGET_MENU_ITEM_ENABLED),
};

struct CWindow file_menu = {
    .top = 20,
    .left = 60,
    .width = 90,
    .height = 80,
    .gadgets = file_menu_items,
    .gadget_count = GADGET_COUNT(file_menu_items),
    .background = BACKGROUND_WHITE
};

struct CGadget about_menu_items[] = {
    GADGET_MENUBAR_ITEM_BOLD(0, 60, "Manager", EVENT_NONE, GADGET_MENU_ITEM_ENABLED),
    GADGET_MENUBAR_ITEM(60, 40, "File", EVENT_NONE, GADGET_MENU_ITEM_ENABLED),
    GADGET_MENUBAR_ITEM(110, 40, "Help", EVENT_NONE, GADGET_MENU_ITEM_DISABLED),
};

struct CWindow init_app_menu = {
    .top = 0,
    .left = 0,
    .width = 1280,
    .height = 20,
    .gadgets = about_menu_items,
    .gadget_count = GADGET_COUNT(about_menu_items)
};

