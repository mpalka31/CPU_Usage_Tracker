/**
 * @file reader.h
 * @author Michał Pałka
 * @brief Header file providing API to interact with reader thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

#define RING_BUFF_SIZE 256      /**< Size of reader -> analyzer ring buffer*/

/**
 * @brief Initialize function for reader thread.
 * 
 */
void readerInit(void);
/**
 * @brief Deinitialize function for reader thread.
 * 
 */
void readerDeinit(void);

/**
 * @brief Initialize function for ring buffer.
 * 
 * Initialize function for ring buffer used to buffered data flows from reader to analyzer thread.
 * It should be called from readerInit function.
 * 
 */
void rawDataRingBufferInit(void);
/**
 * @brief Deinitialize function for ring buffer.
 * 
 * Denitialize function for ring buffer used to buffered data flows from reader to analyzer thread.
 * It should be called from analyzerDeinit function. This function handle memory freed of data not get from buffer at destroy time. 
 */
void rawDataRingBufferDeinit(void);
/**
 * @brief Function using to put data in ring buffer.
 * 
 * Threadsafe API to put data in ring buffer. Data must be allocater using either malloc or calloc. 
 * 
 * @param dataPut       address of pointer to memory where data are stored
 * @param dataSizePut   size of passed data
 */
void rawDataRingBufferPut(char** dataPut, unsigned int dataSizePut);
/**
 * @brief Function using to get data from ring buffer
 * 
 * Threadsafe API to get data from ring buffer. Data stored in buffer are dynamicaly allocatet and they must be freed! 
 * 
 * @param dataGet           address of pointer to store data pointer
 * @return unsigned int     size of get data
 */
unsigned int rawDataRingBufferGet(char** dataGet);

#endif
