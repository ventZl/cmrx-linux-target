#include <cmrx/cmrx.h>
#include <cmrx/clock.h>
#include "fbdev/fbdev.h"
#include <signal.h>
#include <pthread.h>
#include <stdio.h>

void timing_provider_setup(unsigned timeout_ms);

void arch_early_init()
{
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGURG);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    printf("SIGALRM, SIGUSR1 and SIGURG masked\n");
}

int main(int argc, char ** argv)
{
    arch_early_init();
    fbdev_init();
    timing_provider_setup(1000);
    os_start();
}
