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
    
    // pthread_exit(NULL);
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    lineSize = getline(&line, &len, stdin);
    free(line);
    analyzerDeInit();
    readerDeInit();
    return 0;
}
