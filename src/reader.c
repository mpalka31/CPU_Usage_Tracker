/**
 * @file reader.c
 * @author Michał Pałka
 * @brief Implementation of API to interact with reader thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "reader.h"
#include "watchdog.h"
#include "logger.h"

static pthread_t readetThreadID;
void* readerThread(void *arg);

static WatchdogInst* readerWatchdog;
static bool readerInitialized = false;

typedef struct RawDataElement
{
    unsigned int dataSize;
    int dummy;      // dummy int to prevent padding struct warning message
    char* data;
}RawDataElement;

typedef struct RawDataRingBuffer{
    unsigned int head;
    unsigned int tail;
    pthread_mutex_t rawDataMutex;
    sem_t freeSlots_sem;
    sem_t occupiedSlots_sem;
    RawDataElement elements[RING_BUFF_SIZE];
} RawDataRingBuffer;
static RawDataRingBuffer rawDataRingBuffer;

static char* rawData;
static unsigned int rawDataSize;
static FILE* procStat = NULL;

void readerInit(void){
    readerWatchdog = watchdogRegister("READER");
    readerWatchdog->enable = true;
    rawData=NULL;
    rawDataRingBufferInit();
    pthread_create(&readetThreadID, NULL, readerThread, (void*)NULL);
    readerInitialized = true;
    logINFO("READER", "initialized");
}

void readerDeinit(void){
    if(!readerInitialized) return;
    pthread_cancel(readetThreadID);
    if(rawData!=NULL) free(rawData);
    if(procStat!=NULL) fclose(procStat);
    readerInitialized = false;
    logINFO("READER", "deinitialized");
}

static void readData(void){
    procStat = fopen("/proc/stat", "r");
    if(procStat != NULL){
        rawDataSize=0;
        while(fgetc(procStat)!=EOF) {
            rawDataSize++;
        }
        fseek(procStat, 0, SEEK_SET);
        rawData = calloc(rawDataSize, sizeof(char));
        if(fread(rawData, 1, rawDataSize, procStat)<rawDataSize){
            fclose(procStat);
            procStat = NULL;
            free(rawData);
            rawData=NULL;
            logWARNING("READER", "fread() returned size mismatch");
            return;
        }
        fclose(procStat);
        procStat = NULL;
        rawDataRingBufferPut(&rawData, rawDataSize);
        rawData = NULL;
    }else{
        logWARNING("READER", "cannot open /proc/stat");
    }
}

void* readerThread(void *arg){
    (void) arg;
    while (1){
        readerWatchdog->wdt = 0;
        readData();
        usleep(100000);
    }
}

/*********************** RING BUFFER IMPLEMENTATION ***********************/

void rawDataRingBufferInit(void){
    pthread_mutex_init(&(rawDataRingBuffer.rawDataMutex), NULL);
    sem_init(&(rawDataRingBuffer.freeSlots_sem), 0, RING_BUFF_SIZE);
    sem_init(&(rawDataRingBuffer.occupiedSlots_sem), 0, 0);
    rawDataRingBuffer.head=0;
    rawDataRingBuffer.tail=0;
}

void rawDataRingBufferDeinit(void){
    sem_destroy(&(rawDataRingBuffer.freeSlots_sem));
    sem_destroy(&(rawDataRingBuffer.occupiedSlots_sem));
    pthread_mutex_destroy(&(rawDataRingBuffer.rawDataMutex));
    for(int i = 0; i < RING_BUFF_SIZE; i++){
        if(rawDataRingBuffer.elements[i].data!=NULL)
            free(rawDataRingBuffer.elements[i].data);
    }
}

void rawDataRingBufferPut(char** dataPut, unsigned int dataSizePut){
    sem_wait(&(rawDataRingBuffer.freeSlots_sem));
    pthread_mutex_lock(&(rawDataRingBuffer.rawDataMutex));
    rawDataRingBuffer.elements[rawDataRingBuffer.head].data = *dataPut;
    rawDataRingBuffer.elements[rawDataRingBuffer.head].dataSize = dataSizePut;
    rawDataRingBuffer.head = (rawDataRingBuffer.head+1)%RING_BUFF_SIZE;
    sem_post(&(rawDataRingBuffer.occupiedSlots_sem));
    pthread_mutex_unlock(&(rawDataRingBuffer.rawDataMutex));
}

unsigned int rawDataRingBufferGet(char** dataGet){
    sem_wait(&(rawDataRingBuffer.occupiedSlots_sem));
    pthread_mutex_lock(&(rawDataRingBuffer.rawDataMutex));
    unsigned int tempTail = rawDataRingBuffer.tail;
    rawDataRingBuffer.tail = (rawDataRingBuffer.tail+1)%RING_BUFF_SIZE;
    *dataGet = rawDataRingBuffer.elements[tempTail].data;
    rawDataRingBuffer.elements[tempTail].data = NULL;
    sem_post(&(rawDataRingBuffer.freeSlots_sem));
    pthread_mutex_unlock(&(rawDataRingBuffer.rawDataMutex));
    return rawDataRingBuffer.elements[tempTail].dataSize;
}
