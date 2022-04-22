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

void sigHendler(int sig);

int main(void){
    system("clear");
    printf("PRESS ENTER TO EXIT\n\n");
    
    readerInit();
    analyzerInit();
    printerInit();
    watchdogInit();

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigHendler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    
    char c=getc(stdin);
    
    printf("\nUSER TERMINATE\n");
    readerDeinit();
    analyzerDeinit();
    printerDeinit();
    watchdogDeinit();
    return 0;
}

void sigHendler(int sig){
    if((sig==SIGINT)||(sig==SIGTERM)){
        printf("\nSIGNAL TERMINATE\n");
        readerDeinit();
        analyzerDeinit();
        printerDeinit();
        watchdogDeinit();
        exit(0);
    }
}