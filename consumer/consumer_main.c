#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../utilities/time_util.h"
#include "../utilities/exp_dist.h"
#include "../utilities/check_mod.c"
#include "../data_structures/shared_memory.h"
#include "../data_structures/circular_buffer.h"

shared_memory *sm_ptr;
int total_message_processed = 0;
//long begin_time, end_time = 0;
crono Cronometers;

int main(int argc, char **argv)
{
    char usage_msj[] = "Usage: ./consumer_main sharedMemoryId(int) consumptionTime(int)\n %s";

    //Setting random for exponential distribution
    exponential_dist_setup();
   
    if(argc != 3){
        fprintf(stderr, usage_msj, "Parameter count!\n");
        return EXIT_FAILURE;
    }

    //get the id from argv
    int shmid = atoi(&(*argv[1]));

    // consumption time (we ask the user for seconds but need useconds)
    // check in the project spec what is needed here!!!
    long pt = atoi(&(*argv[2]));

    if (shmid < 1 || pt < 1)
    {
        fprintf(stderr, usage_msj, "All values must be a positive number greater than zero!\n");
        return EXIT_FAILURE;
    }

    init_cronometer(&Cronometers.execution_time);
    init_cronometer(&Cronometers.time_spent_in_exp_dist_delay);
    init_cronometer(&Cronometers.time_spent_waiting_shared_memory);
    cronometer_start(&Cronometers.start_execution_time);

    pt = pt * 1000000;

    // semaphores configuration
    struct sembuf sb = {0, -1, 0};
    struct sembuf sb1 = {1, -1, 0};
    struct sembuf sb2 = {2, 1, 0};

    // shmat to attach to shared memory
    sm_ptr = (shared_memory *)shmat(shmid, 0, 0);

    sm_ptr->consumers_count++;
    int pid = getpid();
    bool working = true;
    //gettimeofday(&tv, NULL);
    //begin_time = tv.tv_sec;

    while (working) // this condition must be changed so it loops until the consumer is said to no longer consume
    {
        //calculating random delay with exponential distribution with mean of "pt"
        int exp_delay = exponential_dist(pt);
        cronometer_start(&Cronometers.start_delay_time); //timestamp from the instant when delay starts
        usleep(exp_delay);             // the value should be taken from the console arguments (check project spec for details)
        cronometer_end(&Cronometers.end_delay_time); //timestamp from the instant when delay ends

        //save the time spent in last delay
        save_time_spent(&Cronometers.start_delay_time, &Cronometers.end_delay_time, &Cronometers.time_spent_in_exp_dist_delay);


        //timestamp from the instant when request to shared memory starts
        cronometer_start(&Cronometers.start_waiting_time);
        semop(sm_ptr->semid, &sb1, 1); // prevents underflow
        semop(sm_ptr->semid, &sb, 1);  // controls buffer access

        //timestamp from the instant when request to shared memory starts
        cronometer_end(&Cronometers.end_waiting_time);
        save_time_spent(&Cronometers.start_waiting_time, &Cronometers.end_waiting_time, &Cronometers.time_spent_waiting_shared_memory);

        // attach buffer and messages structures to the shared memory
        sm_ptr->buffer = (circular_buffer *)shmat(sm_ptr->cb_shmid, NULL, 0);
        (*sm_ptr->buffer).messages = (message *)shmat(sm_ptr->m_shmid, NULL, 0);

        // takes the first item from the buffer
        message m = CB_pop(sm_ptr->buffer, pid, sm_ptr->consumers_count, sm_ptr->producers_count);
        total_message_processed++;
        //printf("mensaje: %d \n",m.key);

        //Randall
        //cuando al leer un mensaje este incluya una llave (numero entero entre 0 y 4) que sea igual al processid  o PID del consumidor modulo 5
        //hasta que lean un mensaje especial que indique que el sistema se deba suspender
        if (is_MyPid_equal_key(pid, m.key) || m.key == 6)
        {
            if (m.key == 6)
            {
                printf("--> Consumer PID: %d, Recibi mensaje propio de apagarse, mensaje: %d \n", pid, m.key);
            }
            else
            {
                printf("--> Consumer PID: %d, Mensaje mod 5 mi pid: %d, mensaje %d \n", pid, (get_mod(pid)), m.key);
            }

            working = false;
            //gettimeofday(&tv, NULL);
            cronometer_end(&Cronometers.end_execution_time);
            //printf("The value for end execution is : %ld", Cronometers.end_execution_time);
            save_time_spent(&Cronometers.start_execution_time, &Cronometers.end_execution_time, &Cronometers.execution_time);

            //end_time = tv.tv_sec;
            //printf("Consumer PID: %d, Total message: %d, Seconds activity: %ld \n", pid, total_message_processed, (end_time - begin_time));
            sm_ptr->consumers_count--;
            printf("===============================================================\n");
            printf("=== Report of consumer with id: %5d                       ===\n",pid);
            printf("=== Number of messages consumed: %5d                      ===\n",total_message_processed);
            printf("=== Total execution time is: %5d (s)                      ===\n",Cronometers.execution_time);
            printf("=== Total time spent in delay is: %5d (s)                 ===\n",Cronometers.time_spent_in_exp_dist_delay);
            printf("=== Total time spent waiting for shared mem is: %5d (s)   ===\n",Cronometers.time_spent_waiting_shared_memory);
           printf("===============================================================\n");
        }

        sb.sem_op = 1;
        semop(sm_ptr->semid, &sb, 1);
        semop(sm_ptr->semid, &sb2, 1); // prevents overflow
    }

    //detach from shared memory
    shmdt(sm_ptr);

    return 0;
}