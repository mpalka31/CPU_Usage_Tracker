#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>

#define INFO_LEN    64

typedef struct WatchdogInst WatchdogInst;
typedef struct WatchdogInst{
    unsigned int wdt;
    bool enable;
    char info[INFO_LEN];
    struct WatchdogInst* nextWatchdogInst;
}WatchdogInst;


void watchdogInit(void);
void watchdogDeinit(void);
WatchdogInst* watchdogRegister(const char * info);

#endif