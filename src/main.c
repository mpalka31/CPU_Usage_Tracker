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

void sigHendler(int sig);

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
