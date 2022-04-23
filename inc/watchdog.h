/**
 * @file watchdog.h
 * @author Michał Pałka
 * @brief Header file providing API to interact with watchdog thread.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>

#define INFO_LEN    64      /**< Maximum length of watchdog instance info string*/

typedef struct WatchdogInst WatchdogInst;
/**
 * @brief Structure to represent watchdog instantion
 * 
 */
typedef struct WatchdogInst{
    bool enable;    /**< variable determining if watchdog should check watchdog timer of particular instantion, by default set to false (not check)*/
    bool dummy1;    /**< dummy bool to prevent padding struct warning message*/
    bool dummy2;    /**< dummy bool to prevent padding struct warning message*/
    bool dummy3;    /**< dummy bool to prevent padding struct warning message*/
    unsigned int wdt;       /**< watchdog timer it increments every 1ms and shoud be zeros by controlled thread*/
    char info[INFO_LEN];    /**< info string, typically thread name*/
    WatchdogInst* nextWatchdogInst;     /**< pointer to next registered watchdog instance*/
}WatchdogInst;

/**
 * @brief Initialize function for printer thread.
 * 
 */
void watchdogInit(void);
/**
 * @brief Deinitialize function for printer thread.
 * 
 */
void watchdogDeinit(void);
/**
 * @brief Function using to register watchdog instantion.
 * 
 * @param info              info string, typically thread name
 * @return WatchdogInst*    pointer to created watchdog instance 
 */
WatchdogInst* watchdogRegister(const char * info);

#endif
