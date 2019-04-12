#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 

    //attach to shared memory
    sm_ptr = (shared_memory*) shmat(shmid, 0,0); 

    //initialize counters
    sm_ptr->consumers_count = 0;
    sm_ptr->producers_count = 0;
    
    CB_init(&sm_ptr->buffer, 5);

    printf("shared memory created! your id is %d\n", shmid);

    return 0;
}