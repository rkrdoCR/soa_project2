#include <check_mod.h>


int get_mod(int n){
   return n % 5;
}

//
int is_MyPid_equal_key (int my_pid, int key){
    
   return (0 == (key - get_mod(my_pid))? 1:0);
    
}


 
