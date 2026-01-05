#pragma once

#define GADGET_TITLE_BAR(_width) \
{ .type = GADGET_TITLE_BAR, .top = 0, .left = 0, .width = (_width), .height = 22, .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE, .state = 0 }

#define GADGET_PANEL(_left, _top, _width, _height, _color) \
{ .type = GADGET_PANEL, .top = (_top), .left = (_left), .width = (_width), .height = (_height), .fg_color = (_color) }

#define GADGET_TEXT(_left, _top, _width, _height, _text, _color, _attrs) \
{ .type = GADGET_TEXT, .top = (_top), .left = (_left), .width = (_width), .height = (_height), .flags = GADGET_FLAG_ENABLED, .state = 0, .text = (_text), .fg_color = (_color), .text_flags = (_attrs) }

#define GADGET_BUTTON(_left, _top, _width, _height, _text, _attrs, _event) \
{ .type = GADGET_BUTTON, .top = (_top), .left = (_left), .width = (_width), .height = (_height), .flags = (_attrs), .state = 0, .text = (_text), .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE, .event_id = (_event) }

#define GADGET_PIXMAP(_left, _top, _width, _height, _pixmap_id) \
{ .type = GADGET_PIXMAP, .top = (_top), .left = (_left), .width = (_width), .height = (_height), .flags = GADGET_FLAG_ENABLED, .state = 0, .pixmap_id = (_pixmap_id) }

#define _GADGET_MENUBAR_ITEM(_left, _width, _text, _event, _flags, _text_attr) \
{ .type = GADGET_MENU_ITEM, .top = 0, .left = (_left), .width = (_width), .height = 20, .text = (_text), .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE | (_text_attr), .flags = (_flags), .text_margin_horiz = 5, .sub_menu_id = WINDOW_NONE, .event_id = (_event) }

#define GADGET_MENUBAR_ITEM(_left, _width, _text, _event, _flags) \
_GADGET_MENUBAR_ITEM(_left, _width, _text, _event, _flags, 0)

#define GADGET_MENUBAR_ITEM_BOLD(_left, _width, _text, _event, _flags) \
_GADGET_MENUBAR_ITEM(_left, _width, _text, _event, _flags, TEXT_BOLD)

#define GADGET_MENUWINDOW_ITEM(_no, _width, _text, _event, _flags) \
{ .type = GADGET_MENU_ITEM, .top = (_no) * 20, .left = 0, .width = (_width), .height = 20, .text = (_text), .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE, .flags = (_flags), .text_margin_horiz = 5, .sub_menu_id = WINDOW_NONE, .event_id = (_event) }

#define GADGET_MENU_ITEM_ENABLED (GADGET_FLAG_ACTIVABLE | GADGET_FLAG_ENABLED)
#define GADGET_MENU_ITEM_DISABLED 0

#define GADGET_COUNT(_gadgets) (sizeof(_gadgets)/sizeof(_gadgets[0]))

#define WINDOW_MENUBAR(_items) \
{ .top = 0, .left = 0, .width = SCREEN_WIDTH, .height = 20, .gadgets = (_items), .gadget_count = GADGET_COUNT(_items) }

#define WINDOW_MENUWINDOW(_left, _width, _items) \
{ .top = 20, .left = (_left), .width = (_width), .height = GADGET_COUNT(_items) * 20, .gadgets = _items, .gadget_count = GADGET_COUNT(_items), .background = BACKGROUND_WHITE }
