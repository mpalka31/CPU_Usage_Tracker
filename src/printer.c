#include "printer.h"


pthread_t printerThreadID;

void* printerThread(void *arg);
struct timeval tv;
unsigned long timestamp1;// = 1000000 * tv.tv_sec + tv.tv_usec;
unsigned long timestamp2;

void printerInit(){
    pthread_create(&printerThreadID, NULL, printerThread, (void*)NULL);
}

void printerDeInit(){
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
        while((tempNodeData = cpuUsageQueueRead())!=NULL){
            if(cnt==0){
                cores = tempNodeData->cores;
                tempUsage = calloc(cores, sizeof(CpuUsage));
            }
            for(int i = 0; i < cores; i++){
                memcpy(tempUsage[i].name, (tempNodeData->cpuUsageTab_p)[i].name, CPU_ID_LEN);
                tempUsage[i].usage=tempUsage[i].usage+(tempNodeData->cpuUsageTab_p)[i].usage;
            }
            cnt++;
            cpuUsageQueueDelete();
        }
        for(int i = 0; i<cores; i++){
            tempUsage[i].usage=tempUsage[i].usage/(float)cnt;
            printf("%s\t%.2f\n", tempUsage[i].name, tempUsage[i].usage);
        }
        free(tempUsage);
        timestamp2 = 1000000 * tv.tv_sec + tv.tv_usec;
        printf("%ld\n", timestamp2-timestamp1);
    }
}