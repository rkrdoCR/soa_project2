#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "data_structures/shared_memory.h"
#include "data_structures/circular_buffer.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{
    printf("hello finalizer\n");

    //get the id from argv
    int shmid = atoi(&(*argv[1])); 
  
    // shmat to attach to shared memory 
    sm_ptr = (shared_memory*) shmat(shmid, 0,0);

    //free(sm_ptr->buffer);

    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 

    return 0;
}