
#include <calltrace/calltrace.h>
#include <stdio.h>
#include <stdarg.h>
#include <calltrace/buffer.h>

function_signature_void(void, function1);
function_signature_void(void, function2);
function_signature_void(void, function3);
function_signature(void, Log, const char* format, ...);

#define function1() define_alias_function_void_macro(function1)
#define function2() define_alias_function_void_macro(function2)
#define function3() define_alias_function_void_macro(function3)
#define Log(...)	define_alias_function_macro(Log, __VA_ARGS__)



function_signature(void, Log, const char* format, ...)
{
	CALLTRACE_BEGIN();
	va_list args; 
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	puts(calltrace_string());
	CALLTRACE_END();
}

function_signature_void(void, function1)
{
	CALLTRACE_BEGIN();

	Log("Example Error: This is an example error!\n");


	Log("Example Warning: This is an example warning!\n");
	CALLTRACE_END();
}

function_signature_void(void, function2)
{
	CALLTRACE_BEGIN();

	function1();
	Log("Example Error: This is another example error!\n");
	CALLTRACE_END();
}

function_signature_void(void, function3)
{
	CALLTRACE_BEGIN();

	function2();

	CALLTRACE_END();
}

int main(int argc, char** argv)
{
	void* integers = buffer_init(sizeof(int)); 
	int value = 100;
	buffer_push(integers, &value);
	value = 102;
	buffer_push(integers, &value); 

	for(int i = 0; i < buffer_count(integers); i++)
		printf("Value: %u\n", ((int*)integers)[i]);

	buffer_destroy(integers);
	calltrace_init();
	
	function3();

	calltrace_log();

	calltrace_terminate();
	return 0;
}