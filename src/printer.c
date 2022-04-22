#include "printer.h"


pthread_t printerThreadID;

void* printerThread(void *arg);
struct timeval tv;
unsigned long timestamp1;
unsigned long timestamp2;

extern pthread_mutex_t cpuUsageNodeDataMutex;

void printerInit(){
    pthread_create(&printerThreadID, NULL, printerThread, (void*)NULL);
}

void printerDeinit(){
    pthread_cancel(printerThreadID);
}

void* printerThread(void *arg){
    while(1){
        usleep(1e6-(timestamp2-timestamp1));
        timestamp1 = 1000000 * tv.tv_sec + tv.tv_usec;
        CpuUsageNodeData* tempNodeData;
        CpuUsage* tempUsage;
        int cnt=0;
        int cores;
        pthread_mutex_lock(&cpuUsageNodeDataMutex);
        while((tempNodeData = cpuUsageQueueRead())!=NULL){
            if(cnt==0){
                cores = tempNodeData->cores;
                tempUsage = calloc(cores, sizeof(CpuUsage));
            }
            for(int i = 0; i < cores; i++){
                memcpy(tempUsage[i].name, (tempNodeData->cpuUsageTab_p)[i].name, CPU_ID_LEN);
                if(!isnan((tempNodeData->cpuUsageTab_p)[i].usage)){
                    tempUsage[i].usage=tempUsage[i].usage+(tempNodeData->cpuUsageTab_p)[i].usage;
                }
            }
            cnt++;
            cpuUsageQueueDelete();
        }
        pthread_mutex_unlock(&cpuUsageNodeDataMutex);
        system("clear");
        printf("PRESS ENTER TO EXIT\n\n");
        for(int i = 0; i<cores; i++){
            tempUsage[i].usage=tempUsage[i].usage/(float)cnt;
            printf("%s\tusage:\t%.2f%%\n", tempUsage[i].name, tempUsage[i].usage);
        }
        free(tempUsage);
        timestamp2 = 1000000 * tv.tv_sec + tv.tv_usec;
    }
}