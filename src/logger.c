#include "logger.h"
#include "watchdog.h"

#define FILE_PATH   "./CUT_LOG.txt"

static pthread_t loggerThreadID;
void* loggerThread(void *arg);

static WatchdogInst* loggerWatchdog;
static bool loggerInitialized = false;
static pthread_mutex_t loggerMutex = PTHREAD_MUTEX_INITIALIZER;

static FILE* logFile = NULL;

void loggerInit(void){
    pthread_mutex_lock(&loggerMutex);
    loggerWatchdog = watchdogRegister("LOGGER");
    loggerWatchdog->enable=true;
    pthread_create(&loggerThreadID, NULL, loggerThread, (void*)NULL);
    loggerInitialized = true;
    
}

void loggerDeinit(void){
    if(!loggerInitialized) return;
    pthread_cancel(loggerThreadID);
    if(logFile != NULL) fclose(logFile);
    logFile = NULL;
    pthread_mutex_destroy(&loggerMutex);
    loggerInitialized = false;
}

void logINFO(const char* source, const char* INFO){
    pthread_mutex_lock(&loggerMutex);
    if(logFile!=NULL)
        fprintf(logFile, "INFO %s:\t%s\n", source, INFO);
    pthread_mutex_unlock(&loggerMutex);
}

void logWARNING(const char* source, const char* WARNING){
    pthread_mutex_lock(&loggerMutex);
    if(logFile!=NULL)
        fprintf(logFile, "WARNING %s:\t%s\n", source, WARNING);
    pthread_mutex_unlock(&loggerMutex);
}

void logERROR(const char* source, const char* ERROR){
    pthread_mutex_lock(&loggerMutex);
    if(logFile==NULL)
        fprintf(logFile, "ERROR %s:\t%s\n", source, ERROR);
    pthread_mutex_unlock(&loggerMutex);
}

void* loggerThread(void *arg){
    (void) arg;
    while((logFile = fopen(FILE_PATH, "w")) == NULL);
    pthread_mutex_unlock(&loggerMutex);
    while (1){
        sleep(1);
        loggerWatchdog->wdt = 0;
    }
    
}
