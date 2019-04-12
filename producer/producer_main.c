#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;
message *msg;

int main(int argc, char **argv)
{
    //get the id from argv
    int shmid = atoi(&(*argv[1])); 
  
    // shmat to attach to shared memory 
    sm_ptr = (shared_memory*) shmat(shmid, 0,0);
    sm_ptr->buffer = (circular_buffer*)shmat(sm_ptr->cb_shmid, NULL, 0);
    (*sm_ptr->buffer).messages = (message*)shmat(sm_ptr->m_shmid, NULL, 0);
  
    sm_ptr->producers_count++;

    int pid = getpid();

    printf("%d\n", sm_ptr->producers_count);
    printf("PID = %d\n", pid);

    CB_push(sm_ptr->buffer, pid, 1);
      
    //detach from shared memory  
    shmdt(sm_ptr); 
     
    return 0;
}