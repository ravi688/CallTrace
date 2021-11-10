
#include <calltrace/calltrace.h>
#include <stdio.h>
#include <stdarg.h>
#include <calltrace/buffer.h>

function_signature_void(void, function1);
function_signature_void(void, function2);
function_signature_void(void, function3);

#define function1() define_alias_function_void_macro(function1)
#define function2() define_alias_function_void_macro(function2)
#define function3() define_alias_function_void_macro(function3)



function_signature_void(void, function1)
{
	CALLTRACE_BEGIN();

	log_msg("Example Error: This is an example error!\n");


	log_msg("Example Warning: This is an example warning!\n");
	CALLTRACE_END();
}

function_signature_void(void, function2)
{
	CALLTRACE_BEGIN();

	function1();
	log_msg("Example Error: This is another example error!\n");
	CALLTRACE_END();
}

function_signature_void(void, function3)
{
	CALLTRACE_BEGIN();

	function2();

	CALLTRACE_RETURN();
}

int main(int argc, char** argv)
{	
	function3();
	return 0;
}