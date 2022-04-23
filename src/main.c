/**
 * @file main.c
 * @author Michał Pałka
 * @brief It is CUT (CPU Usage Tracker) application. It constantly scans uour CPU usage and display it every second in terminal window.
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "watchdog.h"
#include "logger.h"

/**
 * @brief sigHandler is function which handle intercepted signal. It can handle both SIGTERM and SIGINT signal.
 * 
 * @param sig Signal identifier
 */
void sigHendler(int sig);

/**
 * @brief Enty point of application. It initialize all components and wait for user input to exit.
 * 
 * @return int Application exit code. If closed correctly equals to 0, -1 on error exit. 
 */
int main(void){
    system("clear");
    printf("PRESS ENTER TO EXIT\n\n");
    
    loggerInit();
    watchdogInit();
    readerInit();
    analyzerInit();
    printerInit();

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigHendler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    
    int c = getc(stdin);
    (void)c;
    
    logINFO("MAIN", "\tENTER pressed, application terminate");
    readerDeinit();
    analyzerDeinit();
    printerDeinit();
    watchdogDeinit();
    loggerDeinit();
    return 0;
}

void sigHendler(int sig){
    if((sig==SIGINT)||(sig==SIGTERM)){
        logWARNING("MAIN", "SIGNAL intercepted, application rerminate");
        readerDeinit();
        analyzerDeinit();
        printerDeinit();
        watchdogDeinit();
        loggerDeinit();
        exit(0);
    }
}
