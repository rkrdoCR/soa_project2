#include "time_util.h"

char* get_current_date()
{
    char* result;

    time_t t =time(NULL);
    struct tm *tm = localtime(&t);
    result = asctime(tm);
    
    return result;
}