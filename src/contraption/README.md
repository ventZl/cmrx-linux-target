Windowing server
================

This is a small windowing server running on top of framebuffer devices. This window server performs completely server-side GUI rendering and large part of UI interaction. Applications are not involved in the interaction until they order window server to send them an event in case of certain event.
