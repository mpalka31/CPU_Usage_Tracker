#include "reader.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

pthread_t readetThreadID;

char* rawData;
pthread_mutex_t rawDataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t rawDataReadyCond = PTHREAD_COND_INITIALIZER;
bool rawDataReady = false;
int rawDataSize;

void readerInit(void){
    pthread_create(&readetThreadID, NULL, readerThread, (void*)NULL);
}

void readData(void){
    FILE* ptr;
    ptr = fopen("/proc/stat", "r");
    if(ptr != NULL){
        pthread_mutex_lock(&rawDataMutex);
        rawDataSize=0;
        while(fgetc(ptr)!=EOF) {
            rawDataSize++;
        }
        fseek(ptr, 0, SEEK_SET);
        rawData = (char*)malloc(rawDataSize);
        fread(rawData, 1, rawDataSize, ptr);
        fclose(ptr);
        // printf("%s", rawData);
        pthread_cond_signal(&rawDataReadyCond);
        // rawDataReady = true;
        pthread_mutex_unlock(&rawDataMutex);
        // free(rawData);
    }
}

void* readerThread(void *arg){
    while (1){
        readData();
        sleep(1);
    }
}