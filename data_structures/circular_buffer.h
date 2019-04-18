#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include "data_structures/message.h"
#include "message.h"

typedef struct
{
    int size;
    int start;
    int count;

    message *messages;
} circular_buffer;

void CB_init(circular_buffer *cb, int size);
void CB_push(circular_buffer *cb, int pid, int k);
message CB_pop(circular_buffer *cb, int pid);
bool CB_full(circular_buffer *cb);
bool CB_empty(circular_buffer *cb);

#endif