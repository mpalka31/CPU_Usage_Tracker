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
    bool enable;
    // 3 dummy bool to prevent padding struct warning message
    bool dummy1;
    bool dummy2;
    bool dummy3;
    unsigned int wdt;
    char info[INFO_LEN];
    WatchdogInst* nextWatchdogInst;
}WatchdogInst;


void watchdogInit(void);
void watchdogDeinit(void);
WatchdogInst* watchdogRegister(const char * info);

#endif
