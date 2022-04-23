#include "watchdog.h"
#include "printer.h"
#include "analyzer.h"
#include "reader.h"
#include "logger.h"

#define WATCHDOG_TIMEOUT    (2e3)

void watchdogHandle(void);

static unsigned int watchdogCNT=0;

static pthread_t watchdogThreadID;
void* watchdogThread(void *arg);

static WatchdogInst* watchdogHead = NULL;
static bool watchdogInitialized = false;

void watchdogInit(void){
    pthread_create(&watchdogThreadID, NULL, watchdogThread, (void*)NULL);
    watchdogInitialized = true;
    logINFO("WATCHDOG", "initialized");
}

void watchdogDeinit(void){
    if(!watchdogInitialized) return;
    pthread_cancel(watchdogThreadID);
    for(unsigned int i = 0; i < watchdogCNT; i++){
        WatchdogInst* tmpWtdPTR = watchdogHead->nextWatchdogInst;
        free(watchdogHead);
        watchdogHead = tmpWtdPTR;
    }
    watchdogInitialized = false;
    logINFO("WATCHDOG", "deinitialized");
}

WatchdogInst* watchdogRegister(const char * info){
    WatchdogInst* tmpWtdPTR = calloc(1, sizeof(WatchdogInst));
    memcpy(tmpWtdPTR->info, info, strlen(info));
    tmpWtdPTR->nextWatchdogInst = watchdogHead;
    watchdogHead = tmpWtdPTR;
    watchdogCNT++;
    return watchdogHead;
}

void* watchdogThread(void *arg){
    (void) arg;
    while(1){
        usleep(1e3);
        watchdogHandle();
    }
}

void watchdogHandle(void){
        WatchdogInst* watchdogPtr = watchdogHead;
        for(unsigned int i = 0; i < watchdogCNT; i++){
            if(watchdogPtr->enable){
                if(watchdogPtr->wdt>WATCHDOG_TIMEOUT){
                    logERROR(watchdogPtr->info, "WDT overflow");
                    logERROR("WATCHDOG", "WDT overflow detected, wxit witch -1 code");
                    readerDeinit();
                    analyzerDeinit();
                    printerDeinit();
                    watchdogDeinit();
                    loggerDeinit();
                    exit(-1);
                }
            }
            watchdogPtr->wdt++;
            watchdogPtr = watchdogPtr->nextWatchdogInst;
        }
}
