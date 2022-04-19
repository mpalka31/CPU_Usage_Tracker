#ifndef ANALYZER_H
#define ANALYZER_H


typedef struct CpuUsage {
    char name[24];
    float usage;
} CpuUsage;
CpuUsage* cpuUsageTab;

void analyzerInit(void);
void analyzerDeInit(void);
void* analyzerThread(void *arg);

#endif