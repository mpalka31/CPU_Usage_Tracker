#include "analyzer.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

pthread_t analyzerThreadID;

extern char* rawData;
extern pthread_mutex_t rawDataMutex;
extern pthread_cond_t rawDataReadyCond;
extern bool rawDataReady;
extern int rawDataSize;

void analyzerInit(void){
    pthread_create(&analyzerThreadID, NULL, analyzerThread, (void*)NULL);
}

void* analyzerThread(void *arg){
    while(1){
        pthread_mutex_lock(&rawDataMutex);
        while(rawData==NULL){
            pthread_cond_wait(&rawDataReadyCond, &rawDataMutex);
        }
        FILE* tempData = fmemopen(rawData, rawDataSize, "r");
        while(1){
            char cpu[24];
            int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
            int bytes = fscanf(tempData, "%s %d %d %d %d %d %d %d %d %d %d", cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
            if(strncmp(cpu, "cpu", 3)!=0) break;
            
            printf("%s %d %d %d %d %d %d %d %d %d %d\n", cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);
        }
        fclose(tempData);
        printf("\n");
        // printf("%s", rawData);
        free(rawData);
        rawData=NULL;
        // rawDataReady = false;
        sleep(1);
        pthread_mutex_unlock(&rawDataMutex);
    }
}