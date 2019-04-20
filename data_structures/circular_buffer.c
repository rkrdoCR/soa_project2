#include <string.h>

#include "../utilities/time_util.h"
#include "circular_buffer.h"
#include "message.h"

message *msg;


void CB_init(circular_buffer *cb, int size)
{
    cb->size = size;
    cb->start = 0;
    cb->count = 0;
}

void CB_push(circular_buffer *cb, int pid, int k)
{
    int index;
    if (CB_full(cb))
    {
        printf("Buffer overflow\n");
    }
    else
    {
        index = cb->start + cb->count++;
        if (index >= cb->size)
        {
            index = 0;
        }
        cb->messages[index].key = k;
        cb->messages[index].pid = pid;
        strcpy(cb->messages[index].date, get_current_date());

        printf("Producer PID: %d, has pushed a message to index: %d at %s\n",
        pid, index, cb->messages[index].date);
    }
}

message CB_pop(circular_buffer *cb, int pid, int total_consumer, int total_producer)
{
    message element;
    if (CB_empty(cb))
    {
        printf("Buffer underflow\n");
        return element;
    }
    else
    {
        element = cb->messages[cb->start];
        cb->start++;
        cb->count--;
        if (cb->start == cb->size)
        {
            cb->start = 0;
        }

        printf("Consumer PID: %d, has pulled a message from index: %d, message: %d, Total consumer: %d, at %s \n",
        pid, cb->start, cb->messages[cb->start].key, total_consumer, cb->messages[cb->start].date );

        return element;
    }
}

bool CB_empty(circular_buffer *cb)
{
    if (cb->count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CB_full(circular_buffer *cb)
{
    if (cb->count == cb->size)
    {
        return true;
    }
    else
    {
        return false;
    }
}