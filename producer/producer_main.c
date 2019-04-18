#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "utilities/time_util.h"
#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;
message *msg;

int main(int argc, char **argv)
{
    char usage_msj[] = "Usage: ./producer_main sharedMemoryId(int) productionTime(int)\n %s";
    /*
    ***********Add this when we stop using launch.json for debug***********
    if(argc != 2){
        fprintf(stderr, usage_msj, "Parameter count!\n");
        return EXIT_FAILURE;
    }*/

    // get the id from argv
    int shmid = atoi(&(*argv[1])); //TODO change this to index 0

    // production time (we ask the user for seconds but need useconds)
    // we need to integrate Marco's computation for exponential distribution
    // the param index for the time will eventually be different
    long pt = atoi(&(*argv[3]));//TODO change this to index 1
    pt = pt * 1000000;

    if(shmid < 1 || pt < 1){
        fprintf(stderr, usage_msj, "All values must be a positive number greater than zero!\n");
        return EXIT_FAILURE;
    }

    // semaphores configuration
    struct sembuf sb  = {0, -1, 0};
    struct sembuf sb1 = {1, 1, 0};
    struct sembuf sb2 = {2, -1, 0};

    // shmat to attach to shared memory
    sm_ptr = (shared_memory *)shmat(shmid, 0, 0);

    sm_ptr->producers_count++;
    int pid = getpid();

    while (1) // this condition should be changed so the loop goes until the producer is said to no longer produce
    {
        usleep(pt);
        semop(sm_ptr->semid, &sb2, 1); // prevents overflow
        semop(sm_ptr->semid, &sb, 1);  // controls buffer access

        // attach buffer and messages structures to the shared memory
        sm_ptr->buffer = (circular_buffer *)shmat(sm_ptr->cb_shmid, NULL, 0);
        (*sm_ptr->buffer).messages = (message *)shmat(sm_ptr->m_shmid, NULL, 0);

        // append message to the buffer
        CB_push(sm_ptr->buffer, pid, 1);

        sb.sem_op = 1;
        semop(sm_ptr->semid, &sb, 1);
        semop(sm_ptr->semid, &sb1, 1); // prevents underflow
    }

    //detach from shared memory
    shmdt(sm_ptr);

    return 0;
}