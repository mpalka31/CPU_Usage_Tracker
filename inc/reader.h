#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

void readerInit(void);
void readerDeInit(void);
void* readerThread(void *arg);

#endif