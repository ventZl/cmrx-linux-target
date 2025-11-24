# Amiga Intuition Windowing System Guide

## Overview

Amiga Intuition was a sophisticated windowing system for the Amiga microcomputer (1985+) that pioneered several advanced concepts for home computers of its era. Unlike traditional windowing systems that required applications to handle expose events and manual redraws, Intuition used a **retained-mode architecture** where the window server maintained complete UI state and could automatically redraw damaged areas without application intervention.

## Core Architecture

### Retained Mode Design
- **Server-side scene graph**: Intuition maintained complete window and gadget hierarchies in server memory
- **No expose events**: Applications didn't receive damage notifications - the server handled all redraws automatically
- **Automatic refresh**: When windows overlapped or moved, Intuition redrew affected areas from retained state
- **Declarative UI**: Applications described desired interface elements rather than managing their lifecycle

### Key Components
- **Intuition.library**: Main windowing and event management
- **Layers.library**: Handled window overlapping, clipping, and damage calculation
- **Graphics.library**: Low-level drawing and rastport management

## Window Creation Process

### The Transfer Model
Applications constructed complete window definitions in their own memory, then transferred ownership to Intuition in a single call:

1. **Build window structure** with all properties and gadget hierarchies
2. **Call OpenWindow()** to transfer the entire structure to server memory  
3. **Server takes ownership** and manages the window from retained copies
4. **Application becomes client** receiving event notifications about user interactions

```c
struct NewWindow nw = {
    0, 0, 320, 200,           // Position and size
    -1, -1,                   // DetailPen, BlockPen
    IDCMP_CLOSEWINDOW | IDCMP_GADGETUP,  // Event types to receive
    WFLG_CLOSEGADGET | WFLG_DRAGBAR,     // Window flags
    &firstGadget,             // First gadget in linked list
    NULL,                     // CheckMark image
    "My Window",              // Title
    NULL, NULL,               // Screen, BitMap
    100, 50, 640, 400,        // Min/max dimensions
    WBENCHSCREEN              // Screen type
};

window = OpenWindow(&nw);
```

## Gadgets (Widgets)

### Gadget Types (Amiga 1.x)
- **BOOLGADGET**: Simple buttons with press/release behavior
- **STRGADGET**: Single-line text input fields with basic editing
- **PROPGADGET**: Sliders and scrollbars with proportional positioning
- **System Gadgets**: Close, depth (front/back), sizing, drag bar

### Gadget Construction
Applications manually constructed gadget structures with complete specifications:

```c
struct Gadget myButton = {
    NULL,                    // NextGadget (linked list pointer)
    10, 20,                  // LeftEdge, TopEdge coordinates
    80, 25,                  // Width, Height in pixels
    GFLG_GADGHCOMP,         // Behavior flags
    GACT_RELVERIFY,         // Activation conditions
    GTYP_BOOLGADGET,        // Gadget type
    &buttonBorder,          // Visual representation
    &buttonSelectedBorder,  // Selected state appearance
    &buttonText,            // Associated text
    NULL                    // Application user data
};
```

### Gadget Hierarchies
Gadgets were organized as linked lists attached to windows:
- Multiple gadgets linked via `NextGadget` pointers
- Complete hierarchy passed to `OpenWindow()`
- Server maintained copies and handled all interaction

### String Gadget Limitations (1.x)
- **Single-line only**: No multiline text support
- **No scrolling**: Text longer than gadget width was clipped
- **Fixed buffer size**: Had to pre-allocate maximum string length
- **Basic editing**: Insert, delete, cursor movement, simple selection
- **No clipboard operations**: No copy/paste functionality

## Layout and Positioning

### Manual Positioning System
Intuition provided **no automatic layout calculation**:
- All gadget positions specified as exact pixel coordinates
- Developers responsible for calculating spacing and alignment
- No constraint-based positioning or automatic sizing
- Limited "gravity" features (relative positioning to window edges)

### Window and Gadget Overlapping
- **Full overlap support** through layers.library integration
- **Automatic clipping** of overlapped regions
- **Z-order management** for both windows and gadgets
- **Damage calculation** and refresh without application intervention

## Event System

### Event Types
Applications received `IntuiMessage` events for user interactions:

- `IDCMP_GADGETUP/DOWN`: Gadget press/release events
- `IDCMP_MOUSEMOVE`: Mouse movement (if requested)  
- `IDCMP_MOUSEBUTTONS`: Raw mouse button events
- `IDCMP_RAWKEY/VANILLAKEY`: Keyboard input
- `IDCMP_CLOSEWINDOW`: Window close requests
- `IDCMP_NEWSIZE`: Window resize events
- `IDCMP_MENUPICK`: Menu selection events

### Event Message Structure
```c
struct IntuiMessage {
    ULONG Class;              // Event type
    USHORT Code;              // Additional event code
    USHORT Qualifier;         // Modifier keys (Shift/Ctrl/Alt)
    APTR IAddress;            // Pointer to activated object (gadget)
    WORD MouseX, MouseY;      // Mouse coordinates
    ULONG Seconds, Micros;    // Timestamp
    struct Window *IDCMPWindow; // Source window
};
```

## Menu System

### Menu Construction
Menus used a three-level hierarchical structure built from linked data structures:

```c
struct Menu {
    struct Menu *NextMenu;      // Menu bar chain
    WORD LeftEdge, TopEdge;     // Position in menu bar
    WORD Width, Height;         // Dimensions
    BYTE *MenuName;             // Menu title
    struct MenuItem *FirstItem; // First menu item
};

struct MenuItem {
    struct MenuItem *NextItem;   // Item chain
    WORD LeftEdge, TopEdge;     // Position in menu
    WORD Width, Height;         // Item dimensions
    APTR ItemFill;              // Text or image
    BYTE Command;               // Keyboard shortcut
    struct MenuItem *SubItem;   // Submenu items
};
```

### Menu Limitations (1.x)
- **Single menu strip per window**: Only one menu bar possible
- **Right mouse button only**: No left-click or context menus
- **No automatic layout**: All menu item positions manually calculated
- **Global per window**: Same menu regardless of cursor position
- **No dynamic switching**: Menu strip statically attached to window

### Menu Activation
- `SetMenuStrip(window, menustrip)` attached menus to windows
- Automatic activation based on window focus
- Applications received `IDCMP_MENUPICK` messages with menu selection details
- No application intervention needed for basic menu display

## Advanced Features

### Window Focus Integration
- Only active window's menu strip available
- Automatic menu switching when changing window focus
- No menu conflicts between multiple open windows
- Declarative approach - set once, system manages thereafter

### Memory Management
- Applications could discard original structures after `OpenWindow()`
- Server maintained complete copies of all UI elements
- Automatic cleanup when windows closed
- No manual resource tracking required for basic UI elements

## Historical Context and Evolution

### Amiga 1.x Limitations
- Very limited gadget set requiring custom implementations for complex UI
- Primitive layout capabilities compared to modern systems
- No high-level constructor APIs for UI elements
- Manual structure building for all interface elements

### Amiga 2.0+ Improvements
- **GadTools library**: Higher-level gadget creation functions
- **BOOPSI**: Object-oriented gadget system
- **New gadget types**: Listview, cycle, checkbox, radio buttons
- **Better layout helpers** and automatic sizing
- **Enhanced visual styling** with 3D appearance

### Significance
For a home computer in 1985, Intuition's retained-mode architecture with automatic refresh was remarkably sophisticated. While the layout capabilities were primitive, the core concept of server-side UI state management was ahead of its time and influenced later windowing systems.

The system demonstrated that complex windowing functionality was possible on resource-constrained hardware through careful architectural design, even if the developer experience required significant manual work compared to modern GUI frameworks.