#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "reader.h"
#include "analyzer.h"
#include "printer.h"

void sigHendler(int sig);

int main(void){
    system("clear");
    printf("PRESS ENTER TO EXIT\n\n");
    
    readerInit();
    analyzerInit();
    printerInit();

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigHendler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    
    char c=getc(stdin);
    
    readerDeinit();
    analyzerDeinit();
    printerDeinit();
    printf("\nUSER TERMINATE\n");
    return 0;
}

void sigHendler(int sig){
    if((sig==SIGINT)||(sig==SIGTERM)){
        readerDeinit();
        analyzerDeinit();
        printerDeinit();
        printf("\nSIGNAL TERMINATE\n");
        exit(0);
    }
}