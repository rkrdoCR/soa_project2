#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{ 
    //get the buffersize from argv
    int buffer_size = atoi(&(*argv[2]));

    // ftok to generate unique key 
    key_t key = ftok("creator_main.c",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 

    // attach to shared memory
    sm_ptr = (shared_memory*) shmat(shmid, 0,0); 

    // store the ids of the circular buffer and the message array, they will be used
    // on the producer and consumer to attach them to the their respective space in the shared memory
    sm_ptr->cb_shmid = shmget(1010, sizeof(circular_buffer), IPC_CREAT|0666);
    sm_ptr->m_shmid = shmget(1011, sizeof(message), IPC_CREAT|0666);
    sm_ptr->semid = semget(1012, 3, 0666 | IPC_CREAT);
    
    // attach this process to the shared memory. This will also initialize their memory spaces 
    sm_ptr->buffer = (circular_buffer*)shmat(sm_ptr->cb_shmid, NULL, 0);
    (*sm_ptr->buffer).messages = (message*)shmat(sm_ptr->m_shmid, NULL, 0);

    // initialize counters
    sm_ptr->consumers_count = 0;
    sm_ptr->producers_count = 0;

    // initialize semaphores. Will use a semaphore set of 3 as follow:
    //      1. semaphore 0: controls buffer access
    //      2. semaphore 1: prevents underflow
    //      3. semaphore 2: prevents overflow

    // semaphore 0 initialization
    sm_ptr->sem1_arg.array = (unsigned short*)shmat(shmget(1013, sizeof(unsigned short), IPC_CREAT|0666),0,0);
    sm_ptr->sem1_arg.buf = (struct semid_ds*)shmat(shmget(1014, sizeof(struct semid_ds), IPC_CREAT|0666),0,0);
    sm_ptr->sem1_arg.val = 1;
    semctl(sm_ptr->semid, 0, SETVAL, sm_ptr->sem1_arg);

    // semaphore 1 initialization
    // sm_ptr->sem2_arg.val = 0;
    // semctl(sm_ptr->semid, 1, SETVAL, sm_ptr->sem2_arg);

    // semaphore 2 initialization
    // sm_ptr->sem3_arg.val = buffer_size;
    // semctl(sm_ptr->semid, 2, SETVAL, sm_ptr->sem3_arg);    
    
    // circular buffer initialization
    CB_init(sm_ptr->buffer, buffer_size);

    printf("shared memory created! your id is %d\n", shmid);

    return 0;
}