
#include <calltrace/calltrace.h>
#include <calltrace/debug.h>
#include <stdlib.h>
#include <memory.h>

#define BUFFER_OFFSET 16 /*[u64, capacity], [u64, count]*/
#define BUFFER_ELEMENT_SIZE 24 /*[u64, line no], [pointer, functionName], [pointer, fileName]*/
static void* buffer = NULL + BUFFER_OFFSET;

#define buffer_capacity() (*__buffer_capacity())
#define buffer_count() (*__buffer_count())

static u64* __buffer_capacity()
{
	if((buffer - BUFFER_OFFSET) == NULL) return 0; 
	else return (u64*)(buffer - BUFFER_OFFSET);
}
static u64* __buffer_count()
{
	if((buffer - BUFFER_OFFSET) == NULL) return 0;
	else return (u64*)(buffer - BUFFER_OFFSET + 8);
}
static void buffer_init()
{
	buffer = malloc(BUFFER_OFFSET) + BUFFER_OFFSET;
	buffer_capacity() = 0;
	buffer_count() = 0;
}
static void buffer_ensure_capacity(u64 capacity)
{
	u64 new_capacity = (buffer_capacity() == 0) ? 1 : buffer_capacity();
	while(new_capacity < capacity)
		new_capacity *= 2;
	if(new_capacity != buffer_capacity())
	{
		buffer = realloc(buffer - BUFFER_OFFSET, new_capacity * BUFFER_ELEMENT_SIZE + BUFFER_OFFSET) + BUFFER_OFFSET;
		buffer_capacity() = new_capacity;
	}
}
static void buffer_destroy()
{
#if defined(CALLTRACE_DEBUG) && defined(LOG_DEBUG)
	if((buffer - BUFFER_OFFSET) == NULL)
		log_wrn("(buffer - BUFFER_OFFSET) == NULL, make sure to call calltrace_init first!");
#endif
	free(buffer - BUFFER_OFFSET);
}

void calltrace_buffer_push(callinfo_t info)
{
	buffer_ensure_capacity(buffer_count() + 1);
	memcpy(buffer + buffer_count(), &info, BUFFER_ELEMENT_SIZE);
	buffer_count()++;
}
void calltrace_buffer_pop()
{
	buffer_count()--;
	memset(buffer, 0, BUFFER_ELEMENT_SIZE);
}
void calltrace_init()
{
#if defined(CALLTRACE_DEBUG) && defined(LOG_DEBUG)
	if(sizeof(callinfo_t) != BUFFER_ELEMENT_SIZE)
		log_fetal_err("sizeof(callinfo_t) != BUFFER_ELEMENT_SIZE, Make sure to update BUFFER_ELEMENT_SIZE too!");
#endif
	buffer_init();
}
void calltrace_terminate() { buffer_destroy(); }

