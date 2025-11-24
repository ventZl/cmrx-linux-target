#include "contraption.h"
#include <cmrx/ipc/isr.h>
#include <stdbool.h>

// Lowest possible IRQ

void IRQ_15()
{
    display.pointer = true;
    display.button = true;
    isr_notify_object(&display);
}
