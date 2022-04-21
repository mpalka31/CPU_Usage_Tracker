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
    signal(SIGINT, sigHendler);
    signal(SIGTERM, sigHendler);
    
    char c=getc(stdin);

    analyzerDeInit();
    readerDeInit();
    printerDeInit();
    printf("\nUSER TERMINATE\n");
    return 0;
}

void sigHendler(int sig){
    if((sig==SIGINT)||(sig==SIGTERM)){
        analyzerDeInit();
        readerDeInit();
        printerDeInit();
        printf("\nSIGNAL TERMINATE\n");
        exit(0);
    }
}