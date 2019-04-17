#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{ 
    char usage_msj[] = "Usage: ./consumer_main sharedMemoryId(int) consumptionTime(int)\n %s";
    /*
    ***********Add this when we stop using launch.json for debug***********
    if(argc != 2){
        fprintf(stderr, usage_msj, "Parameter count!\n");
        return EXIT_FAILURE;
    }*/

    //get the id from argv
    int shmid = atoi(&(*argv[1])); //TODO change this to index 0

    // consumption time (we ask the user for seconds but need useconds)
    // check in the project spec what is needed here!!!
    long pt = atoi(&(*argv[3]));//TODO change this to index 2
    pt = pt * 1000000;

    if(shmid < 1 || pt < 1){
        fprintf(stderr, usage_msj, "All values must be a positive number greater than zero!\n");
        return EXIT_FAILURE;
    }

    // semaphores configuration
    struct sembuf sb  = {0, -1, 0};
    struct sembuf sb1 = {1, -1, 0};
    struct sembuf sb2 = {2, 1, 0};
  
    // shmat to attach to shared memory 
    sm_ptr = (shared_memory*) shmat(shmid, 0,0);

    sm_ptr->consumers_count++;
    int pid = getpid();
    
    while(1) // this condition must be changed so it loops until the consumer is said to no longer consume
    {
        usleep(2000000); // the value should be taken from the console arguments (check project spec for details)
        semop(sm_ptr->semid, &sb1, 1); // prevents underflow
        semop(sm_ptr->semid, &sb, 1);  // controls buffer access

        // attach buffer and messages structures to the shared memory
        sm_ptr->buffer = (circular_buffer*)shmat(sm_ptr->cb_shmid, NULL, 0);
        (*sm_ptr->buffer).messages = (message*)shmat(sm_ptr->m_shmid, NULL, 0);
  
        // takes the first item from the buffer
        message m = CB_pop(sm_ptr->buffer, pid);

        sb.sem_op = 1;
        semop(sm_ptr->semid, &sb, 1);
        semop(sm_ptr->semid, &sb2, 1); // prevents overflow
        
    }
      
    //detach from shared memory  
    shmdt(sm_ptr);
     
    return 0;
}