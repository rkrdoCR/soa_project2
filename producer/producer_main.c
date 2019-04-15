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
    //get the id from argv
    int shmid = atoi(&(*argv[1]));

    //production time (we ask the user for seconds but need useconds)
    long pt = atoi(&(*argv[3]));
    pt = pt * 1000000;

    struct sembuf sb = {0, -1, 0};

    // shmat to attach to shared memory
    sm_ptr = (shared_memory *)shmat(shmid, 0, 0);

    sm_ptr->producers_count++;
    int pid = getpid();

    while (1)
    {
        usleep(pt);
        semop(sm_ptr->semid, &sb, 1);

        sm_ptr->buffer = (circular_buffer *)shmat(sm_ptr->cb_shmid, NULL, 0);
        (*sm_ptr->buffer).messages = (message *)shmat(sm_ptr->m_shmid, NULL, 0);

        CB_push(sm_ptr->buffer, pid, 1);

        sb.sem_op = 1;
        semop(sm_ptr->semid, &sb, 1);
    }

    //detach from shared memory
    shmdt(sm_ptr);

    return 0;
}