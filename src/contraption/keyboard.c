#include "contraption.h"
#include <cmrx/ipc/isr.h>
#include <stdbool.h>
#include <stdio.h>

// Second lowest possible IRQ

void IRQ_14()
{
    display.keypress = true;
    isr_notify_object(&display);
}

