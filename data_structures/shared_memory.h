#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>

#include "circular_buffer.h"

#define MESSAGE_MAX_SIZE (size_t)4096

typedef struct
{
    int producers_count;
    int consumers_count;
    circular_buffer *buffer;
} shared_memory;

int shm_create();

#endif