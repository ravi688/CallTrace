
#include <stdio.h>
#include <calltrace/calltrace.h>

function_signature_void(void, function1);
function_signature_void(void, function2);
function_signature_void(void, function3);

#define function1() define_alias_function_void_macro(function1)
#define function2() define_alias_function_void_macro(function2)
#define function3() define_alias_function_void_macro(function3)

void __log(const char* format, ...)
{
	// vprintf()
}

function_signature_void(void, function1)
{
	CALLTRACE_BEGIN();
	__log("Hello %s World", "In the new");
	CALLTRACE_END();
}

function_signature_void(void, function2)
{
	CALLTRACE_BEGIN();

	function1();
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
	calltrace_init(); 
	
	function1();

	calltrace_terminate();
	return 0;
}