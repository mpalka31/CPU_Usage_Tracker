/**
 * @file printer.h
 * @author Michał Pałka
 * @brief Header file providing API to interact with printer thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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

/**
 * @brief Initialize function for printer thread.
 * 
 */
void printerInit(void);
/**
 * @brief Denitialize function for printer thread.
 * 
 */
void printerDeinit(void);


#endif
