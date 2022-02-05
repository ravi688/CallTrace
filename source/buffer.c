#include <calltrace/buffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


#define BUFFER_OFFSET 24 /*[u64, capacity], [u64, count], [u64, element_size]*/

CALLTRACE_API u64* __buffer_element_size(void* buffer)
{
	if((buffer - BUFFER_OFFSET) == NULL) return 0; 
	else return (u64*)(buffer - BUFFER_OFFSET + 16);	
}

CALLTRACE_API u64* __buffer_capacity(void* buffer)
{
	if((buffer - BUFFER_OFFSET) == NULL) return 0; 
	else return (u64*)(buffer - BUFFER_OFFSET);
}
CALLTRACE_API u64* __buffer_count(void* buffer)
{
	if((buffer - BUFFER_OFFSET) == NULL) return 0;
	else return (u64*)(buffer - BUFFER_OFFSET + 8);
}
CALLTRACE_API void buffer_log(void* buffer)
{
	printf(
		"Buffer:\n"
		"\tcount: %u\n"
		"\tcapacity: %u\n"
		"\telement_size: %u\n",
		buffer_count(buffer),
		buffer_capacity(buffer),
		buffer_element_size(buffer)
	);
}
CALLTRACE_API void* buffer_init(u64 element_size)
{
	void* buffer = malloc(BUFFER_OFFSET) + BUFFER_OFFSET;
	memset(buffer - BUFFER_OFFSET, 0, BUFFER_OFFSET);
	buffer_element_size(buffer) = element_size;
	return buffer;
}
CALLTRACE_API void* buffer_ensure_capacity(void* buffer, u64 capacity)
{
	u64 new_capacity = (buffer_capacity(buffer) == 0) ? 1 : buffer_capacity(buffer);
	while(new_capacity < capacity)
		new_capacity *= 2;
	if(new_capacity != buffer_capacity(buffer))
	{
		void* new_buffer = realloc(buffer - BUFFER_OFFSET, new_capacity * buffer_element_size(buffer) + BUFFER_OFFSET) + BUFFER_OFFSET;
		buffer_capacity(new_buffer) = new_capacity;
		return new_buffer;
	}
	return buffer;
}
CALLTRACE_API void buffer_clear(void* buffer)
{
	buffer_count(buffer) = 0;
}
CALLTRACE_API void buffer_destroy(void* buffer)
{
#if defined(CALLTRACE_DEBUG)
	if((buffer - BUFFER_OFFSET) == NULL)
	{
		printf("[Error]: (buffer - BUFFER_OFFSET) == NULL, make sure to call calltrace_init first!");
		exit(0);
	}
#endif
	free(buffer - BUFFER_OFFSET);
}
CALLTRACE_API void* buffer_push(void* buffer, void* data)
{
	void* new_buffer = buffer_ensure_capacity(buffer, buffer_count(buffer) + 1);
	memcpy(new_buffer + buffer_element_size(new_buffer) * buffer_count(new_buffer), data, buffer_element_size(new_buffer));
	buffer_count(new_buffer)++;
	return new_buffer;
}
CALLTRACE_API void buffer_pop(void* buffer)
{
	buffer_count(buffer)--;
	memset(buffer + buffer_element_size(buffer) * buffer_count(buffer), 0, buffer_element_size(buffer));
}