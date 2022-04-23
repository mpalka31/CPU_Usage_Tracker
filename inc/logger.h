#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void loggerInit(void);
void loggerDeinit(void);
void logINFO(const char* source, const char* INFO);
void logWARNING(const char* source, const char* WARNING);
void logERROR(const char* source, const char* ERROR);

#endif
