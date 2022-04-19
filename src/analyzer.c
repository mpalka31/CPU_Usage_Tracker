#include "analyzer.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


typedef struct CpuStat{
    char name[24];
    unsigned long long int idleTime;
    unsigned long long int nonIdleTime;
    unsigned long long int totalTime;
}CpuStat;

CpuStat* cpuStatTabPrew;
CpuStat* cpuStatTabCur;
static bool firstRun = true;
static int cpus = 0;
pthread_t analyzerThreadID;


extern char* rawData;
extern pthread_mutex_t rawDataMutex;
extern pthread_cond_t rawDataReadyCond;
extern int rawDataSize;

bool cpuParser(FILE* tempData, int id, CpuUsage** cpuUsageTab_p);

void analyzerInit(void){
    pthread_create(&analyzerThreadID, NULL, analyzerThread, (void*)NULL);
}

void analyzerDeInit(void){
    if(cpuStatTabPrew!=NULL) free(cpuStatTabPrew);
    if(cpuStatTabCur!=NULL) free(cpuStatTabCur);
    if(cpuUsageTab!=NULL) free(cpuUsageTab);
}

void* analyzerThread(void *arg){
    while(1){
        pthread_mutex_lock(&rawDataMutex);
        while(rawData==NULL){
            pthread_cond_wait(&rawDataReadyCond, &rawDataMutex);
        }
        FILE* tempData = fmemopen(rawData, rawDataSize, "r");
        
        int id=0;
        while(1){
            if(!cpuParser(tempData, id, &cpuUsageTab)) break;
            id++;
        }
        firstRun = false;
        fclose(tempData);
        free(rawData);
        rawData=NULL;
        
        sleep(1);
        pthread_mutex_unlock(&rawDataMutex);
    }
}

bool cpuParser(FILE* tempData, int id, CpuUsage** cpuUsageTab_p){
    if(firstRun){
        cpus = id+1;
        cpuStatTabCur = realloc(cpuStatTabCur, cpus*sizeof(CpuStat));
        cpuStatTabPrew = realloc(cpuStatTabPrew, cpus*sizeof(CpuStat));
        *cpuUsageTab_p = realloc(*cpuUsageTab_p, cpus*sizeof(CpuUsage));
    }
    char cpu[24];
    unsigned long long int user, nice, system, idle, ioWait, irq, softIrq, steal, guest, guestNice;
    int matched = fscanf(tempData, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", cpu, &user, &nice, &system, &idle, &ioWait, &irq, &softIrq, &steal, &guest, &guestNice);
    if((strncmp(cpu, "cpu", 3)!=0)||(matched!=11)) return false;
    
    memcpy(&cpuStatTabPrew[id], &cpuStatTabCur[id], sizeof(CpuStat));
    cpuStatTabCur[id].idleTime = idle + ioWait;
    cpuStatTabCur[id].nonIdleTime = user + nice + system + irq + softIrq + steal;
    cpuStatTabCur[id].totalTime =  cpuStatTabCur[id].idleTime + cpuStatTabCur[id].nonIdleTime;
    (*cpuUsageTab_p)[id].usage = (float)(cpuStatTabCur[id].nonIdleTime-cpuStatTabPrew[id].nonIdleTime)*100.0/(float)(cpuStatTabCur[id].totalTime-cpuStatTabPrew[id].totalTime);

    printf("%s\t%.2f\n", cpu, (*cpuUsageTab_p)[id].usage);
    return true;
}