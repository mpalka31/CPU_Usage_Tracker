#ifndef ANALYZER_H
#define ANALYZER_H

#define CPU_ID_LEN    24

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "reader.h"

typedef struct CpuUsage {
    char name[CPU_ID_LEN];
    float usage;
} CpuUsage;

typedef struct CpuUsageNodeData{
    int cores;
    CpuUsage* cpuUsageTab_p;
}CpuUsageNodeData;

void cpuUsageQueueAdd(CpuUsageNodeData** newNodeData);
CpuUsageNodeData* cpuUsageQueueRead(void);
int cpuUsageQueueDelete(void);


void analyzerInit(void);
void analyzerDeinit(void);
void* analyzerThread(void *arg);

#endif