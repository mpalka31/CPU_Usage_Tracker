#include "analyzer.h"
#include "reader.h"
#include "watchdog.h"
#include "logger.h"

typedef struct CpuStat{
    char name[CPU_ID_LEN];
    unsigned long long int idleTime;
    unsigned long long int nonIdleTime;
    unsigned long long int totalTime;
}CpuStat;

static WatchdogInst* analyzerWatchdog;
static bool analyzerInitialized = false;

static CpuStat* cpuStatTabPrew;
static CpuStat* cpuStatTabCur;
static bool firstRun = true;
static unsigned long cpus = 0;
static pthread_t analyzerThreadID;

static FILE* tempData = NULL;
static char* rawDataPtr=NULL;

void getData(void);
bool cpuParser(FILE* tempData, unsigned long id, CpuUsage** cpuUsageTab_p);

void analyzerInit(void){
    pthread_mutex_init(&(cpuUsageNodeDataMutex), NULL);
    analyzerWatchdog = watchdogRegister("ANALYZER");
    analyzerWatchdog->enable = true;
    pthread_create(&analyzerThreadID, NULL, analyzerThread, (void*)NULL);
    analyzerInitialized = true;
    logINFO("ANALYZER", "initialized");
}

void analyzerDeinit(void){
    if(!analyzerInitialized) return;
    pthread_cancel(analyzerThreadID);
    pthread_mutex_destroy(&cpuUsageNodeDataMutex);
    if(tempData!=NULL) fclose(tempData);
    if(cpuStatTabPrew!=NULL) free(cpuStatTabPrew);
    if(cpuStatTabCur!=NULL) free(cpuStatTabCur);
    while(1){
        if(!cpuUsageQueueDelete())break;
    }
    rawDataRingBufferDeinit();
    analyzerInitialized = false;
    logINFO("ANALYZER", "deinitialized");
}

void* analyzerThread(void *arg){
    (void) arg;
    while(1){
        analyzerWatchdog->wdt = 0;
        getData();       
    }
}

void getData(void){
    unsigned int rawDataBuffSize = rawDataRingBufferGet(&rawDataPtr);
    tempData = fmemopen(rawDataPtr, rawDataBuffSize, "r");
    if(tempData == NULL){
        fclose(tempData);
        tempData = NULL;
        free(rawDataPtr);
        rawDataPtr = NULL;
        logWARNING("ANALYZER", "fmemopen() cannot open buffer");
    }
    unsigned long id=0;
    CpuUsageNodeData*  data = calloc(1, sizeof(CpuUsageNodeData));
    data->cpuUsageTab_p = calloc(cpus,sizeof(CpuUsage));
    while(1){

        if(!cpuParser(tempData, id, &(data->cpuUsageTab_p))) break;
        id++;
    }
    data->cores=cpus;

    pthread_mutex_lock(&cpuUsageNodeDataMutex);
    cpuUsageQueueAdd(&data);
    pthread_mutex_unlock(&cpuUsageNodeDataMutex);

    firstRun = false;
    fclose(tempData);
    tempData=NULL;
    free(rawDataPtr);
    rawDataPtr=NULL; 
}

bool cpuParser(FILE* tempData_p, unsigned long id, CpuUsage** cpuUsageTab_p){
    char cpu[CPU_ID_LEN]={0};
    unsigned long long int user, nice, system, idle, ioWait, irq, softIrq, steal, guest, guestNice;
    int matched = fscanf(tempData_p, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", cpu, &user, &nice, &system, &idle, &ioWait, &irq, &softIrq, &steal, &guest, &guestNice);
    if((strncmp(cpu, "cpu", 3)!=0)||(matched!=11)) return false;
    if(firstRun){
        cpus = id+1;
        CpuStat* cpuStatTabCur_temp=calloc(cpus, sizeof(CpuStat));
        memcpy(cpuStatTabCur_temp, cpuStatTabCur, (unsigned long)id*sizeof(CpuStat));
        free(cpuStatTabCur);
        cpuStatTabCur=cpuStatTabCur_temp;

        CpuStat* cpuStatTabPrew_temp=calloc(cpus, sizeof(CpuStat));
        memcpy(cpuStatTabPrew_temp, cpuStatTabPrew, (unsigned long)id*sizeof(CpuStat));
        free(cpuStatTabPrew);
        cpuStatTabPrew=cpuStatTabPrew_temp;

        CpuUsage* cpuUsageTab_temp=calloc(cpus, sizeof(CpuUsage));
        memcpy(cpuUsageTab_temp, *cpuUsageTab_p, (unsigned long)id*sizeof(CpuUsage));
        free(*cpuUsageTab_p);
        *cpuUsageTab_p=cpuUsageTab_temp;
    }
    
    memcpy(&cpuStatTabPrew[id], &cpuStatTabCur[id], sizeof(CpuStat));
    cpuStatTabCur[id].idleTime = idle + ioWait;
    cpuStatTabCur[id].nonIdleTime = user + nice + system + irq + softIrq + steal;
    cpuStatTabCur[id].totalTime =  cpuStatTabCur[id].idleTime + cpuStatTabCur[id].nonIdleTime;
    (*cpuUsageTab_p)[id].usage = (double)(cpuStatTabCur[id].nonIdleTime-cpuStatTabPrew[id].nonIdleTime)*100.0/(double)(cpuStatTabCur[id].totalTime-cpuStatTabPrew[id].totalTime);
    memcpy((*cpuUsageTab_p)[id].name, cpu, CPU_ID_LEN);
    return true;
}

/*************************************************************/

typedef struct CpuUsageNode CpuUsageNode;
typedef struct CpuUsageNode{
    CpuUsageNode* next;
    CpuUsageNodeData* data;
}CpuUsageNode;
static CpuUsageNode* head=NULL;

void cpuUsageQueueAdd(CpuUsageNodeData** newNodeData){
    if(head==NULL){
        head = calloc(1, sizeof(CpuUsageNode));
        head->data=*newNodeData;
        head->next=NULL;
        return;
    }
    CpuUsageNode* tmp_p = head;
    CpuUsageNode* newNode = calloc(1,sizeof(CpuUsageNode));
    newNode->data=*newNodeData;
    newNode->next=NULL;
    while (tmp_p->next!=NULL){
        tmp_p=tmp_p->next;
    }
    tmp_p->next=newNode;
}

CpuUsageNodeData* cpuUsageQueueRead(void){
    if(head==NULL){
        return NULL;
    }
    return head->data;
}

int cpuUsageQueueDelete(void){
    if(head==NULL){
        return 0;
    }else if(head->next==NULL){
        free(head->data->cpuUsageTab_p);
        free(head->data);
        free(head);
        head=NULL;
        return 0;
    }
    CpuUsageNode* tmp_p = head->next;
    free(head->data->cpuUsageTab_p);
    free(head->data);
    free(head);
    head=tmp_p;
    return 1;
}
