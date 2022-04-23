#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

#include "analyzer.h"

void printerInit(void);
void printerDeinit(void);


#endif
