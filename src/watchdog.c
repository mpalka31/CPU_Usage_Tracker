#include "watchdog.h"
#include "printer.h"
#include "analyzer.h"
#include "reader.h"

#define WATCHDOG_TIMEOUT    (2e3)

void watchdogHandle(void);

static unsigned int watchdogCNT=0;

pthread_t watchdogThreadID;
void* watchdogThread(void *arg);
// static WatchdogInst selfWatchdog={0, false, {0}, NULL};   // Unnecessary from practical point of view. Uset only to simplyfiy logic.
static WatchdogInst* watchdogHead = NULL;

void watchdogInit(void){
    pthread_create(&watchdogThreadID, NULL, watchdogThread, (void*)NULL);
}

void watchdogDeinit(void){
    pthread_cancel(watchdogThreadID);
    for(int i = 0; i < watchdogCNT; i++){
        WatchdogInst* tmpWtdPTR = watchdogHead->nextWatchdogInst;
        free(watchdogHead);
        watchdogHead = tmpWtdPTR;
    }

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
    while(1){
        usleep(1e3);
        watchdogHandle();
    }
}

void watchdogHandle(void){
        WatchdogInst* watchdogPtr = watchdogHead;
        for(int i = 0; i < watchdogCNT; i++){
            if(watchdogPtr->enable){
                if(watchdogPtr->wdt>WATCHDOG_TIMEOUT){
                    printf("\nWATCHDOG TERMINATE CAUSED BY:\t%s\n", watchdogPtr->info);
                    readerDeinit();
                    analyzerDeinit();
                    printerDeinit();
                    watchdogDeinit();
                    exit(-1);
                }
            }
            watchdogPtr->wdt++;
            watchdogPtr = watchdogPtr->nextWatchdogInst;
        }
}