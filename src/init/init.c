#include <cmrx/application.h>
#include <stdio.h>
#include <cmrx/sys/syscalls.h>
#include <cmrx/cmrx.h>
#include <cmrx/ipc/notify.h>
#include <cmrx/ipc/mutex.h>
#include <cmrx/ipc/thread.h>
#include <cmrx/ipc/timer.h>
#include <cmrx/ipc/signal.h>
#include <cmrx/ipc/rpc.h>
#include <cmrx/sys/irq.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../contraption/contraption.h"
#include "server.h"

#define FUZZ_DATA_INPUT(name, type) \
static type fuzz_ ## name()\
{\
    type d;\
    if (fread(&d, sizeof(d), 1, stdin) != sizeof(d))\
    {\
        shutdown();\
    }\
    return d;\
}

FUZZ_DATA_INPUT(char, unsigned char);
FUZZ_DATA_INPUT(short, unsigned short);
FUZZ_DATA_INPUT(int, unsigned int);
FUZZ_DATA_INPUT(long, unsigned long);

futex_t futex[255];

int init_fuzz(void * arg)
{
    while (1) {
        if (feof(stdin))
        {
            shutdown();
        }
        unsigned char syscall;
        if (fread(&syscall, 1, 1, stdin) != 1)
        {
            shutdown();
        }
        switch (syscall) {
            case 	SYSCALL_GET_TID:
            {
                int tid = get_tid();
                assert(tid >= 0 && tid < OS_THREADS);

            }
            break;
            case 	SYSCALL_SCHED_YIELD:
            {
                int yield = sched_yield();
                assert(yield == 0);
            }
            break;
/*            case 	SYSCALL_THREAD_CREATE:
            {
                int tid = thread_create()
            }*/
            break;
            case 	SYSCALL_MUTEX_INIT:
            {
                int rv = futex_init(&futex[fuzz_char()]);
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_MUTEX_DESTROY:
            {
                int rv = futex_destroy(&futex[fuzz_char()]);
                assert(rv == 0);
            }

                break;
            case 	SYSCALL_MUTEX_UNLOCK:
            {
                int rv = futex_unlock(&futex[fuzz_char()]);
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_MUTEX_TRYLOCK:
            {
                int rv = futex_trylock(&futex[fuzz_char()]);
                assert(rv == 0);
            }
            break;
            case 	SYSCALL_RPC_CALL:
                break;
            case 	SYSCALL_RPC_RETURN:
                break;
            case 	SYSCALL_THREAD_JOIN:
            {
                int join = thread_join(fuzz_int());
                assert(join == 0);
            }
            break;
            case 	SYSCALL_THREAD_EXIT:
            {
                int rv = thread_exit(fuzz_int());
            }
                break;
            case 	SYSCALL_SETITIMER:
            {
                int rv = setitimer(fuzz_long());
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_USLEEP:
            {
                int rv = usleep(fuzz_long());
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_SIGNAL:
            {
                int rv = signal(fuzz_int(), (void (*)(unsigned int)) fuzz_long());
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_KILL:
            {
                int rv = kill(fuzz_int(), fuzz_int());
                assert(rv == 0);
            }
                break;
            case 	SYSCALL_SETPRIORITY:
            {
                unsigned char new_prio = fuzz_char();
                int prio = setpriority(new_prio);
                assert(prio == new_prio);
            }
                break;
            case 	SYSCALL_RESET:
                break;
            case 	SYSCALL_INIT_WAITABLE_OBJECT:
                break;
            case 	SYSCALL_NOTIFY_OBJECT:
            {
                int rv = notify_object((void *) fuzz_long());
                assert(rv == E_OK);
            }
                break;
            case 	SYSCALL_WAIT_FOR_OBJECT:
            {
                int rv = wait_for_object((void *) fuzz_long(), fuzz_char());
                assert(rv == E_OK || rv == E_TIMEOUT);
            }
                break;
            case 	SYSCALL_ENABLE_IRQ:
            {
                int rv = irq_enable(fuzz_long());
                assert(rv == E_NOTAVAIL);
            }
                break;
            case 	SYSCALL_DISABLE_IRQ:
            {
                int rv = irq_disable(fuzz_long());
                assert(rv == E_NOTAVAIL);
            }
            break;
            case 	SYSCALL_CPUFREQ_GET:
            {
                long freq = get_cpu_freq();
                assert(freq != 0);
            }
                break;
            case 	SYSCALL_GET_MICROTIME:
            {
                uint32_t time = getmicrotime();
                assert(time != 0);
            }
                break;
            case 	SYSCALL_SHUTDOWN:
                shutdown();
                break;
            default:
                abort();
        }
    };
}

int init_main(void * data)
{
    struct CGadget about_gadegets[] = {
        {
            .type = GADGET_BUTTON,
            .top = 370,
            .left = 330/2 - 40,
            .width = 80,
            .height = 30,
            .flags = GADGET_FLAG_ENABLED,
            .state = 0,
            .text = "OK",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE

        },
        {
            .type = GADGET_TITLE_BAR,
            .top = 0,
            .left = 0,
            .width = 330,
            .height = 22,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .state = 0
        },
        {
            .type = GADGET_TEXT,
            .top = 35,
            .left = 10,
            .width = 310,
            .height = 22,
            .flags = GADGET_FLAG_ENABLED,
            .state = 0,
            .text = "Hello Contraption!",
            .fg_color = 0xFF0000,
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE
        }
    };

    struct CWindow about_win = {
        .width = 330,
        .height = 430,
        .top = 90,
        .left = 1280 / 2 - (330 / 2) ,
        .title = "About CMRX",
        .gadgets = about_gadegets,
        .gadget_count = 3
    };

    struct CGadget dialog_gadgets[] = {
        {
            .type = GADGET_TITLE_BAR,
            .width = 400,
            .height = 22,
            .top = 0,
            .left = 0,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .state = 0
        }
    };

    struct CWindow dialog_win = {
        .width = 400,
        .height = 300,
        .top = 100,
        .left = 300,
        .title = "Settings",
        .gadgets = dialog_gadgets,
        .gadget_count = 1
    };

    int dialog_win_id = rpc_call(&display, open_window, &dialog_win);
    int about_win_id = rpc_call(&display, open_window, &about_win);

    struct CGadget file_menu_items[] = {
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "New",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 19,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Open...",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE

        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 39,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Save",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = 0,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 59,
            .left = 0,
            .width = 90,
            .height = 20,
            .text = "Quit",
            .text_flags = TEXT_ALIGN_LEFT | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .text_margin_horiz = 5,
            .sub_menu_id = WINDOW_NONE
        },
    };

    struct CWindow file_menu = {
        .top = 20,
        .left = 0,
        .width = 90,
        .height = 80,
        .gadgets = file_menu_items,
        .gadget_count = 4
    };

    int file_menu_id = rpc_call(&display, open_menu, &file_menu);

    struct CGadget about_menu_items[] = {
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 0,
            .width = 40,
            .height = 20,
            .text = "File",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
            .flags = GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE,
            .sub_menu_id = file_menu_id,
        },
        {
            .type = GADGET_MENU_ITEM,
            .top = 0,
            .left = 39,
            .width = 40,
            .height = 20,
            .text = "Help",
            .text_flags = TEXT_ALIGN_CENTER | TEXT_ALIGN_MIDDLE,
            .flags = 0 //GADGET_FLAG_ENABLED | GADGET_FLAG_ACTIVABLE
        },
    };

    struct CWindow about_menu = {
        .top = 0,
        .left = 0,
        .width = 1280,
        .height = 20,
        .gadgets = about_menu_items,
        .gadget_count = 2
    };



    int about_menu_id = rpc_call(&display, open_menu, &about_menu);
    rpc_call(&display, attach_menu, about_win_id, about_menu_id);

    while (1)
    {
        sleep(1);
/*        int rv = rpc_call(&server, service);
        int my_tid = get_tid();
        printf("I am thread %d\n", my_tid);*/
    }

}

int init_main2(void * data)
{
    (void) data;
    while (1)
    {
        sleep(1);
        int my_tid = get_tid();
    }
}

OS_APPLICATION_MMIO_RANGE(init, 0, 0);
OS_APPLICATION(init);
//OS_THREAD_CREATE(init, init_fuzz, NULL, 64);
OS_THREAD_CREATE(init, init_main, NULL, 64);
OS_THREAD_CREATE(init, init_main2, NULL, 64);
