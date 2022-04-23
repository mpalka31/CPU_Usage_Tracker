/**
 * @file logger.h
 * @author Michał Pałka
 * @brief Header file providing API to interact with logger thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#define FILE_PATH   "./CUT_LOG.txt"     /**< name of file to store logs*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief Initialize function for logger thread.
 * 
 */
void loggerInit(void);
/**
 * @brief Denitialize function for logger thread.
 * 
 */
void loggerDeinit(void);
/**
 * @brief Function using to log regular informations.
 * 
 * Threadsafe function logging regular information messages to file
 * 
 * @param source    source string, typically name of logging thread
 * @param INFO      info message to be logged
 */
void logINFO(const char* source, const char* INFO);
/**
 * @brief Function using to log warning informations.
 * 
 * Threadsafe function logging warning information messages to file
 * 
 * @param source     source string, typically name of logging thread
 * @param WARNING    warning message to be logged
 */
void logWARNING(const char* source, const char* WARNING);
/**
 * @brief Function using to log error informations
 * 
 * Threadsafe function logging error information messages to file
 * 
 * @param source    source string, typically name of logging thread
 * @param ERROR     error message to be logged
 */
void logERROR(const char* source, const char* ERROR);

#endif
