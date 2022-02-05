
#include <calltrace/calltrace.h>
#include <calltrace/buffer.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void* calltrace_buffer = NULL;
static void* strbuffer = NULL;

#ifdef CALLTRACE_DEBUG
__attribute__((constructor)) static void initialize_calltrace() { calltrace_init(); }
__attribute__((destructor)) static void terminate_calltrace() { calltrace_terminate(); }
#endif

CALLTRACE_API void calltrace_buffer_push(callinfo_t info)
{
	calltrace_buffer = buffer_push(calltrace_buffer, &info);
}

CALLTRACE_API void calltrace_buffer_pop()
{
	buffer_pop(calltrace_buffer);
}

CALLTRACE_API void calltrace_init()
{
	calltrace_buffer = buffer_init(sizeof(callinfo_t));
	strbuffer = buffer_init(sizeof(char));
}

CALLTRACE_API void calltrace_terminate()
{ 
	buffer_destroy(calltrace_buffer); 
	buffer_destroy(strbuffer); 
	calltrace_buffer = NULL;
	strbuffer = NULL;
}

CALLTRACE_API void calltrace_log()
{
	#ifdef CALLTRACE_DEBUG
	if(calltrace_buffer == NULL)
	{
		puts("[Warning] calltrace library isn't initialized, message from calltrace_log");
	 	return;
	}
	#endif
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

CALLTRACE_API const char* calltrace_string()
{
	#ifdef CALLTRACE_DEBUG
	if(strbuffer == NULL)
	{
		puts("[Warning] calltrace library isn't initialized, message from calltrace_string");
		return "";
	}
	#endif
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

#ifdef CALLTRACE_DEBUG
CALLTRACE_API function_signature(void, log_msg, const char* format, ...)
{
	CALLTRACE_BEGIN();
	va_list args; 
	printf("[Log]: ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	puts(calltrace_string());
	CALLTRACE_END();
}
CALLTRACE_API function_signature(void, log_err, const char* format, ...)
{
	CALLTRACE_BEGIN();
	va_list args; 
	printf("[Error]: ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	puts(calltrace_string());
	CALLTRACE_END();
}
CALLTRACE_API function_signature(void, log_wrn, const char* format, ...)
{
	CALLTRACE_BEGIN();
	va_list args; 
	printf("[Warning]: ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	puts(calltrace_string());
	CALLTRACE_END();
}
CALLTRACE_API function_signature(void, log_fetal_err, const char* format, ...)
{
	CALLTRACE_BEGIN();
	va_list args;
	printf("[Fetal Error]: ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	puts(calltrace_string());
	CALLTRACE_END();
	exit(0);
}
#endif