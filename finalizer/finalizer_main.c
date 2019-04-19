#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "../data_structures/shared_memory.h"
#include "../data_structures/circular_buffer.h"

shared_memory *sm_ptr;

int main(int argc, char **argv)
{
    int user_input;
    char usage_msj[] = "Usage: ./finalizer_main sharedMemoryId(int)\n %s";
    printf("finalizer program\n");

    //get the id from argv
    int shmid = atoi(&(*argv[1])); //TODO change this to index 0

    if(shmid < 1){
        fprintf(stderr, usage_msj, "All values must be a positive number greater than zero!\n");
        return EXIT_FAILURE;
    }



  
    // shmat to attach to shared memory 
    sm_ptr = (shared_memory*) shmat(shmid, 0,0);

    printf("Please input something to kill producers: \n");
    scanf("%d \n", &user_input);
    printf("You entered: %d\n", user_input);

    (sm_ptr->suspend_producers)=true;

    printf("Producers should be finalizing by now \n");

    

    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 

    return 0;
}