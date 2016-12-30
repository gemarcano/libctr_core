#ifndef CTR_CORE_CIRCULAR_BUFFER_H_
#define CTR_CORE_CIRCULAR_BUFFER_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
	unsigned char *buffer;
	unsigned char *begin, *end;
	size_t size;
	size_t count;
} ctr_core_circular_buffer;

void ctr_core_circular_buffer_initialize(ctr_core_circular_buffer *buffer, size_t size);
bool ctr_core_circular_buffer_push_back(ctr_core_circular_buffer *buffer, char data);
bool ctr_core_circular_buffer_pop_front(ctr_core_circular_buffer *buffer, char *data);
bool ctr_core_circular_buffer_get(ctr_core_circular_buffer *buffer, size_t index, char *data);
size_t ctr_core_circular_buffer_size(ctr_core_circular_buffer *buffer);
size_t ctr_core_circular_buffer_count(ctr_core_circular_buffer *buffer);

#endif//CTR_CORE_CIRCULAR_BUFFER_H_

