#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

#define RING_BUFF_SIZE 256

void rawDataRingBufferInit(void);
void rawDataRingBufferDeinit(void);
void rawDataRingBufferPut(char** dataPut, unsigned int dataSizePut);
unsigned int rawDataRingBufferGet(char** dataGet);

void readerInit(void);
void readerDeinit(void);

#endif