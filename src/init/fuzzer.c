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

/*
 * Fuzzer entrypooint.
 *
 * This is an early implementation of fuzzer entrypoint.
 *
 * Currently deactivated.
 */
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
             *            {
             *                int tid = thread_create()
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

//OS_THREAD_CREATE(init, init_fuzz, NULL, 64);
