#ifndef ANALYZER_H
#define ANALYZER_H


typedef struct CpuStat {
    char name[24];
    float usage;
} CpuStat;
CpuStat* cpuStatTab;

void analyzerInit(void);
void* analyzerThread(void *arg);

#endif