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
    }
}

message CB_pop(circular_buffer *cb)
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