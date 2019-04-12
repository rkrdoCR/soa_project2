#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data_structures/shared_memory.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{ 
    //get the id from argv
    int shmid = atoi(&(*argv[1])); 
  
    // shmat to attach to shared memory 
    sm_ptr = (shared_memory*) shmat(shmid, 0,0);
    sm_ptr->buffer = (circular_buffer*)shmat(sm_ptr->cb_shmid, NULL, 0);
    (*sm_ptr->buffer).messages = (message*)shmat(sm_ptr->m_shmid, NULL, 0);
  
    sm_ptr->consumers_count++;
    message m = CB_pop(sm_ptr->buffer);
  
    printf("Consumers count: %d\n",sm_ptr->consumers_count); 
    printf("PID got from buffer = %d\n", m.pid);
      
    //detach from shared memory  
    shmdt(sm_ptr);
     
    return 0;
}