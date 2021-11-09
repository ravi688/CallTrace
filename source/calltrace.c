
#include <calltrace/calltrace.h>
#include <calltrace/debug.h>
#include <calltrace/buffer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void* calltrace_buffer = NULL;
static void* strbuffer = NULL;

void calltrace_buffer_push(callinfo_t info)
{
	calltrace_buffer = buffer_push(calltrace_buffer, &info);
}

void calltrace_buffer_pop()
{
	buffer_pop(calltrace_buffer);
}

void calltrace_init()
{
	calltrace_buffer = buffer_init(sizeof(callinfo_t));
	strbuffer = buffer_init(sizeof(char));
}

void calltrace_terminate()
{ 
	buffer_destroy(calltrace_buffer); 
	buffer_destroy(strbuffer); 
	calltrace_buffer = NULL;
	strbuffer = NULL;
}

void calltrace_log()
{
	printf(
		"calltrace_log:\n"
		"count: %u\n"
		"capacity: %u\n"
		"element size: %u\n",
		buffer_count(calltrace_buffer), 
		buffer_capacity(calltrace_buffer), 
		buffer_element_size(calltrace_buffer)
	);
} 

const char* calltrace_string()
{
	buffer_clear(strbuffer);
	callinfo_t* buffer = (callinfo_t*)calltrace_buffer;
	const char* callTraceString = "CallTrace:\n";
	u64 callTraceStringLength = strlen(callTraceString);
	strbuffer = buffer_ensure_capacity(strbuffer, buffer_count(strbuffer) + callTraceStringLength);
	strncpy(strbuffer + buffer_count(strbuffer), callTraceString, callTraceStringLength);
	buffer_count(strbuffer) += callTraceStringLength;
	for(u64 i = 0; i < buffer_count(calltrace_buffer); i++)
	{
		strbuffer = buffer_ensure_capacity(strbuffer, buffer_count(strbuffer) + strlen(buffer[i].functionName) + strlen(buffer[i].fileName) + 11 + 32);
		sprintf(strbuffer + buffer_count(strbuffer), "\tline: %u, function: %s, file: %s\n", buffer[i].lineNo, buffer[i].functionName, buffer[i].fileName);
		buffer_count(strbuffer) += strlen(strbuffer + buffer_count(strbuffer));
	}
	return (char*)strbuffer;
}
