#include "exp_dist.h"

int main(int argc, char **argv){
    int value_of_wait = 0;
    int mean=10;
    int iterations=0;

    exponential_dist_setup();


    while(iterations<100){
        value_of_wait=exponential_dist(mean);
        printf("The value of wait is: %d \n",value_of_wait);
        iterations++;
    }

}