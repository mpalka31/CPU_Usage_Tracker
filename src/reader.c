#include "reader.h"

pthread_t readetThreadID;

pthread_mutex_t rawDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t rawDataReadyCond = PTHREAD_COND_INITIALIZER;
char* rawData;
int rawDataSize;
FILE* procStat = NULL;

void readerInit(void){
    rawData=NULL;
    pthread_create(&readetThreadID, NULL, readerThread, (void*)NULL);
}

void readerDeInit(void){
    pthread_cancel(readetThreadID);
    pthread_mutex_destroy(&rawDataMutex);
    pthread_cond_destroy(&rawDataReadyCond);
    if(rawData!=NULL) free(rawData);
    if(procStat!=NULL) fclose(procStat);
}

void readData(void){
    procStat = fopen("/proc/stat", "r");
    if(procStat != NULL){
        pthread_mutex_lock(&rawDataMutex);
        rawDataSize=0;
        while(fgetc(procStat)!=EOF) {
            rawDataSize++;
        }
        fseek(procStat, 0, SEEK_SET);
        rawData = calloc(rawDataSize, 1);
        fread(rawData, 1, rawDataSize, procStat);
        fclose(procStat);
        procStat = NULL;
        pthread_cond_signal(&rawDataReadyCond);
        pthread_mutex_unlock(&rawDataMutex);
    }
}

void* readerThread(void *arg){
    while (1){
        readData();
        usleep(1000000);
    }
}