/**
 * @file analyzer.h
 * @author Michał Pałka
 * @brief Header file providing API to interact with analyzer thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ANALYZER_H
#define ANALYZER_H

#define CPU_ID_LEN    32        /**< Maximum size od CPU core ID (name) parsed from /proc/stat file*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @brief Structure to represent CPU core usage.
 * 
 */
typedef struct CpuUsage {
    char name[CPU_ID_LEN];  /**< core ID (name) parsed from /proc/stat file*/
    double usage;           /**< calculated core usage in %*/
} CpuUsage;
/**
 * @brief Structure to store all parsed cores.
 * 
 */
typedef struct CpuUsageNodeData{
    unsigned long cores;        /**< number of cores parsed from /proc/stat file*/
    CpuUsage* cpuUsageTab_p;    /**< dynamically allocated array of CpuUsage structures representing all cores parsed from /proc/stat file*/
}CpuUsageNodeData;

static pthread_mutex_t cpuUsageNodeDataMutex;

/**
 * @brief Function using to put data in FIFO queue.
 * 
 * Threadsafe API to put data in FIFO queue. All data including members of CpuUsageNodeData structure must be allocater using either malloc or calloc.
 * 
 * @param newNodeData   address of pointer to memory where data are stored
 */
void cpuUsageQueueAdd(CpuUsageNodeData** newNodeData);
/**
 * @brief Function using to read data in FIFO queue.
 * 
 * API to read data in FIFO queue. For threadsafe usage cpuUsageNodeDataMutex must be used to prevent memory deallocation while handling data.
 * If there is no data in queue NULL pointer will be return.
 * 
 * @return CpuUsageNodeData*    pointer to data, NULL pointer if empty
 */
CpuUsageNodeData* cpuUsageQueueRead(void);
/**
 * @brief Function using to delete data in FIFO queue.
 * 
 * Threadsafe API to delete data (head) in FIFO queue.
 * 
 * @return int  1 on success, 0 if queue is empty
 */
int cpuUsageQueueDelete(void);

/**
 * @brief Initialize function for analyzer thread.
 * 
 */
void analyzerInit(void);
/**
 * @brief Denitialize function for analyzer thread.
 * 
 */
void analyzerDeinit(void);

#endif
