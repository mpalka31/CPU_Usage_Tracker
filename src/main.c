#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "reader.h"
#include "analyzer.h"

int main(void){
    readerInit();
    analyzerInit();

    // while (1){
    //     readData();
    //     sleep(1);
    // }
    
    pthread_exit(NULL);
    return 0;
}
