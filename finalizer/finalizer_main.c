#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../utilities/time_util.h"
#include "../data_structures/shared_memory.h"
#include "../data_structures/circular_buffer.h"


shared_memory *sm_ptr;
message *msg;
crono Cronometers;

int main(int argc, char **argv)
{
    init_cronometer(&Cronometers.execution_time);
    init_cronometer(&Cronometers.time_spent_waiting_shared_memory);
    cronometer_start(&Cronometers.start_execution_time);

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

    // semaphores configuration
    struct sembuf sb  = {0, -1, 0};
    struct sembuf sb1 = {1, 1, 0};
    struct sembuf sb2 = {2, -1, 0};

    printf("Please input something to kill producers: \n");
    scanf("%d \n", &user_input);
    printf("You entered: %d\n", user_input);

    (sm_ptr->suspend_producers)=true;

    printf("Producers should be finalizing by now \n");
    
    int pid = getpid();
    while (sm_ptr->consumers_count>0) // this condition should be changed so the loop goes until the producer is said to no longer produce
    {

        //producer is going to wait a exp_delay that is a random number calculated from exponential distribution
        usleep(100000);


        //timestamp from the instant when request to shared memory starts
        cronometer_start(&Cronometers.start_waiting_time);
        semop(sm_ptr->semid, &sb2, 1); // prevents overflow
        semop(sm_ptr->semid, &sb, 1);  // controls buffer access
        //timestamp from the instant when request to shared memory starts
        cronometer_end(&Cronometers.end_waiting_time);
        save_time_spent(&Cronometers.start_waiting_time, &Cronometers.end_waiting_time, &Cronometers.time_spent_waiting_shared_memory);
        

        // attach buffer and messages structures to the shared memory
        sm_ptr->buffer = (circular_buffer *)shmat(sm_ptr->cb_shmid, NULL, 0);
        (*sm_ptr->buffer).messages = (message *)shmat(sm_ptr->m_shmid, NULL, 0);
        
        sm_ptr->total_messages++;
        // append message to the buffer
        CB_push(sm_ptr->buffer, pid, 6,sm_ptr->consumers_count,sm_ptr->producers_count,sm_ptr->total_messages);



        sb.sem_op = 1;
        semop(sm_ptr->semid, &sb, 1);
        semop(sm_ptr->semid, &sb1, 1); // prevents underflow
    }

    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 

    return 0;
}