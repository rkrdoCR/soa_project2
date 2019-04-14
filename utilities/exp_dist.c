#include "exp_dist.h"

//uncomment for debugging
//#define DEBUG 

int exponential_dist_setup(){
    srand(time(NULL));   // Initialization, should only be called once.
    return 0;
}

int exponential_dist(int mean){
    double exponential_dist_double=0;
    int exponential_dist = 0;
    int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
    #ifdef DEBUG
    printf("This is r: %d \n", r);
    #endif
    double linear_dist_prob = (double)r / RAND_MAX;

    #ifdef DEBUG
    printf("This is linear_dist_prob: %f \n", linear_dist_prob);
    #endif
    
    exponential_dist_double=-(double)mean*log(1-linear_dist_prob);
    #ifdef DEBUG
    printf("This is exponential dist double: %f \n", exponential_dist_double);
    #endif
    exponential_dist = (int)exponential_dist_double;
    #ifdef DEBUG
    printf("Exponential_dist: %d \n", exponential_dist);
    #endif
    return exponential_dist;
}