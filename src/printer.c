#include "printer.h"
#include "analyzer.h"
#include "watchdog.h"
#include "logger.h"

static pthread_t printerThreadID;
void* printerThread(void *arg);

void printerPrint(void);

static WatchdogInst* printerWatchdog;
static bool printerInitialized = false;

static struct timeval tv;
static long timestamp1;
static long timestamp2;

void printerInit(){
    printerWatchdog = watchdogRegister("PRINTER");
    printerWatchdog->enable=true;
    pthread_create(&printerThreadID, NULL, printerThread, (void*)NULL);
    printerInitialized = true;
    logINFO("PRINTER", "initialized");
}

void printerDeinit(){
    if(!printerInitialized) return;
    pthread_cancel(printerThreadID);
    printerInitialized = false;
    logINFO("PRINTER", "deinitialized");
}

void* printerThread(void *arg){
    (void) arg;
    while(1){
        printerWatchdog->wdt = 0;
        usleep((unsigned int)1e6-(unsigned int)(timestamp2-timestamp1));
        timestamp1 = 1000000 * tv.tv_sec + tv.tv_usec;
        printerPrint();
        timestamp2 = 1000000 * tv.tv_sec + tv.tv_usec;
    }
}

void printerPrint(void){
    CpuUsageNodeData* tempNodeData = NULL;
    CpuUsage* tempUsage = NULL;
    int cnt=0;
    unsigned long cores=0;
    pthread_mutex_lock(&cpuUsageNodeDataMutex);
    while((tempNodeData = cpuUsageQueueRead())!=NULL){
        if(cnt==0){
            cores = tempNodeData->cores;
            tempUsage = calloc(cores, sizeof(CpuUsage));
        }
        for(unsigned long i = 0; i < cores; i++){
            memcpy(tempUsage[i].name, (tempNodeData->cpuUsageTab_p)[i].name, CPU_ID_LEN);
            if(!isnan((tempNodeData->cpuUsageTab_p)[i].usage)){
                tempUsage[i].usage=tempUsage[i].usage+(tempNodeData->cpuUsageTab_p)[i].usage;
            }else{
                logWARNING("PRINTER", "NaN as CPU usage");
            }
        }
        cnt++;
        cpuUsageQueueDelete();
    }
    pthread_mutex_unlock(&cpuUsageNodeDataMutex);
    system("clear");
    printf("PRESS ENTER TO EXIT\n\n");
    for(unsigned long i = 0; i<cores; i++){
        tempUsage[i].usage=tempUsage[i].usage/(double)cnt;
        printf("%s\tusage:\t%.2f%%\n", tempUsage[i].name, tempUsage[i].usage);
    }
    free(tempUsage);
}
