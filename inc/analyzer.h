#ifndef ANALYZER_H
#define ANALYZER_H

#define CPU_ID_LEN    32

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct CpuUsage {
    char name[CPU_ID_LEN];
    double usage;
} CpuUsage;

typedef struct CpuUsageNodeData{
    unsigned long cores;
    CpuUsage* cpuUsageTab_p;
}CpuUsageNodeData;

void cpuUsageQueueAdd(CpuUsageNodeData** newNodeData);
CpuUsageNodeData* cpuUsageQueueRead(void);
int cpuUsageQueueDelete(void);

static pthread_mutex_t cpuUsageNodeDataMutex;

void analyzerInit(void);
void analyzerDeinit(void);
void* analyzerThread(void *arg);

#endif
