/*

DOCUMENTATION:

About: 
CallTrace is a library to get stack trace of the call hierarchy.
It provides some logging functions to output on standard output.


Getting Started:

1. Declaring functions prototypes:
    Suppose you want to declare function: int get_sum(int a, int b);
    then you have to write: function_signature(int, get_sum, int a, int b);
    and it's corresponding macro_alias: #define get_sum(...) define_alias_function_macro(get_sum, __VA_ARGS__)

    Suppose you want to declare function: const char* get_library_name():
    then you have to write: function_signature_void(const char*, get_library_name);
    and it's corresponding macro_alias: #define get_library_name() define_alias_function_void_macro(get_library_name)
    NOTE: get_library_name has no parameters.

2. Implementing function definitions:
    Suppose you want to implement function: int get_sum(int a, int b);
    then you have to do this:
    function_signature(int, get_sum, int a, int b)
    {
        CALLTRACE_BEGIN();      //Add this if you want to get stack trace of this function's stackframe, it is recommended
        CALLTRACE_RETURN(a + b);        //if you have added CALLTRACE_BEGIN() at the start of this function then it is compulsory to add this at the end of the function.
    }
    OR
    function_signature(int, get_sum, int a, int b)
    {
        CALLTRACE_BEGIN();
        int result = a + b;
        CALLTRACE_END();
        return result;
    }

    Suppose you want to implement function:  const char* get_library_name();
    then you have to do this:
    function_signature_void(const char*, get_library_name)
    {
        CALLTRACE_BEGIN();
        CALLTRACE_RETURN("Hello World\n");
    }

3. Declaring and calling pointers to functions that are implemented with CallTrace
    Suppose you want to declare pointer to a function: int get_sum(int a, int b);
    Then you have to write: func_ptr_sig(int, fptr_get_sum, int a, int b);
    To call this: int result = func_ptr(fptr_get_sum) params(5, 6);
    Or you could have defined an macro alias: #define fptr_get_sum(...) define_alias_function_macro(fptr_get_sum, __VA_ARGS__)
    and then call normally: int result = fptr_get_sum(5, 6);
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(GLOBAL_DEBUG) && !defined(CALLTRACE_DEBUG) 
#define CALLTRACE_DEBUG
#endif

#if defined(GLOBAL_RELEASE) && !defined(CALLTRACE_RELEASE)
#define CALLTRACE_RELEASE
#endif

#if !defined(CALLTRACE_DEBUG) && !defined(CALLTRACE_RELEASE)
#	warning "None of CALLTRACE_DEBUG or CALLTRACE_RELEASE is defined; using CALLTRACE_DEBUG"
#	define CALLTRACE_DEBUG
#elif defined(CALLTRACE_DEBUG) && defined(CALLTRACE_RELEASE)
#	warning "Both of CALLTRACE_DEBUG and CALLTRACE_RELEASE are defined; using CALLTRACE_DEBUG"
#	undef CALLTRACE_RELEASE
#endif

#ifdef CALLTRACE_DEBUG
#	define function_signature_void(return_type, function_name) return_type __##function_name(uint64_t __line__, const char* __function__, const char* __file__)
#	define function_signature(return_type, function_name, ...)  return_type __##function_name(uint64_t __line__, const char* __function__, const char* __file__, __VA_ARGS__)
#	define define_alias_function_void_macro(function_name) __##function_name(__LINE__, __FUNCTION__, __FILE__)
#	define define_alias_function_macro(function_name, ...) __##function_name(__LINE__, __FUNCTION__, __FILE__, __VA_ARGS__)
#   define func_ptr_sig(return_type, function_name, ...) return_type (*__##function_name)(uint64_t, const char*, const char*, __VA_ARGS__)
#   define func_ptr_sig_void(return_type, function_name) return_type (*__##function_name)(uint64_t, const char*, const char*)
#   define func_ptr(function_name) __##function_name
#   define params(...) (__LINE__, __FUNCTION__, __FILE__, __VA_ARGS__)
#   define no_params() (__LINE__, __FUNCTION__, __FILE__)
#elif defined(CALLTRACE_RELEASE)
#	define function_signature_void(return_type, function_name) return_type function_name()
#	define function_signature(return_type, function_name, ...) return_type function_name(__VA_ARGS__)
#	define define_alias_function_macro(function_name, ...) function_name(__VA_ARGS__)
#	define define_alias_function_void_macro(function_name) function_name()
#   define func_ptr_sig(return_type, function_name, ...) return_type (*function_name)(__VA_ARGS__)
#   define func_ptr_sig_void(return_type, function_name) return_type (*function_name)(void)
#   define func_ptr(function_name) function_name
#   define params(...) (__VA_ARGS__)
#   define no_params() ()
#endif

//for u64
#include <calltrace/defines.h>

typedef struct 
{
	u64 lineNo;
	const char* functionName;
	const char* fileName;
} callinfo_t;

/* calltrace_init():
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, however if memory allocation fails (malloc) then it will crash the application.
 * MEMORY OPERATION DESCIPRITIONS:
 *      1. It allocates memory using malloc two times, one for internal stack trace buffer and other for stack trace string buffer.
 * NOTE:
 *      1. If GLOBAL_DEBUG is defined then calltrace library is initialized automatically (using __attribute__((constructor)))
 *      2. Calling calltrace_init() multiple times results in completely new stacktrace and stacktrace log string buffers, and it doesn't frees the previous buffers!
 *      3. If you want to reinitialize the library then you should terminate the first initialization and then call calltrace_init() again.
 */
CALLTRACE_API void calltrace_init();

/* calltrace_terminate():
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      1. [Error]: (buffer - BUFFER_OFFSET) == NULL, make sure to call calltrace_init first!, if you don't initialize the calltrace library first
 *      2. No Errors, however if memory freeing fails (free) then it will crash the application.
 * MEMORY OPERATION DESCRIPTIONS:
 *      1. Frees two memory buffers using free, one for internal stacktrace buffer and other for stacktrace string buffer
 * NOTE:
 *      1. If GLOBAL_DEBUG is defined the calltrace library is terminated automatically (using __attribute__((destructor)))
 *      2. Calling calltrace_terminate() multiple times results in a crash!      
 */
CALLTRACE_API void calltrace_terminate();

/* calltrace_buffer_push(callinfo_t info):
 * DESCRIPTION:
 *      Pushes the callinfo_t struct into the internal stacktrace buffer
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, But it will crash the application if you didn't initialize the library first by calling calltrace_init(). 
 * MEMORY OPERATION DESCRIPTION:
 *      1. It allocates heap memory using malloc if the internal buffers capacity needs to grow to hold the new callinfo_t struct
 *      2. Internal buffer capacity grows double the current capacity.
 * NOTE:
 *      1. Call to this function (calltrace_buffer_push) must be paired with calltrace_buffer_pop()
 *      2. This function must be called at the beginning of a function, though it is not mandatory
 */
CALLTRACE_API void calltrace_buffer_push(callinfo_t info);

/* calltrace_buffer_pop(callinfo_t info):
 * DESCRIPTION:
 *      Pushes the callinfo_t struct into the internal stacktrace buffer
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, But it will crash the application if you didn't initialize the library first by calling calltrace_init(). 
 * MEMORY OPERATION DESCRIPTION:
 *      No heap allocations or free
 * NOTE:
 *      1. Call to this function (calltrace_buffer_pop) must be paired with calltrace_buffer_push()
 *      2. This function must be called at the end of a function, though it is not mandatory
 */
CALLTRACE_API void calltrace_buffer_pop();

/* calltrace_log():
 * DESCRIPTION:
 *      Writes state of the internal stacktrace buffer onto the standard output
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      1. [Warning] calltrace library isn't initialized, message from calltrace_log, if you didn't initialized the library first by calling calltrace_init().
 * MEMORY OPERATION DESCRIPTION:
 *      No heap allocations or free
 * NOTE: 
 *      1. It doesn't crashes the program if library isn't initialized first, writes a warning on the standard output instead.
 */
CALLTRACE_API void calltrace_log();

/* calltrace_string():
 * DESCRIPTION:
 *      Returns a string buffer containg stacktrace info
 * PARAMS:
 *      Nothing
 * RETURNS:
 *      Pointer to internal string buffer
 * ERROR DESCRIPTIONS:
 *      1. [Warning] calltrace library isn't initialized, message from calltrace_string
 * MEMORY OPERATION DESCRIPTION:
 *      1. It allocates heap memory using malloc if the internal string buffer isn't able to hold all the characters.
 *      2. If you call calltrace_string() multiple times then it re-uses the previous allocation if the subsequent call results in the equal or smaller strings.
 *  NOTE: 
 *      1. If the internal string buffer is empty (meaning there is no callinfo_t pushed). It returns "CallTrace:\n" only!
 *      2. If you call this function (calltrace_string) without initializing the library (by calling calltrace_init()) then it write an warning message on stdout.
 *      3. The returned buffer shouldn't be freed, it is being handled by the library itself (when you call calltrace_terminate())
 */
CALLTRACE_API const char* calltrace_string();

#ifdef CALLTRACE_DEBUG
#define CALLTRACE_BEGIN() calltrace_buffer_push((callinfo_t) { __line__, __function__, __file__ })
#define CALLTRACE_END() calltrace_buffer_pop()
#define CALLTRACE_RETURN(...) do { CALLTRACE_END(); return __VA_ARGS__; } while(0)
#elif defined(CALLTRACE_RELEASE)
#define CALLTRACE_BEGIN()
#define CALLTRACE_END()
#define CALLTRACE_RETURN(...) return __VA_ARGS__
#endif


#ifdef CALLTRACE_DEBUG
/* log_err(const char* format, ...):
 * DESCRIPTION:
 *      Writes the formated string prefixed with "[Error]: " and suffixed with calltrace_string()
 * PARAMS:
 *      1. format: format of the output string just like in printf
 *      2. (...) : variable arguements just like in printf
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, but it will crash the application with the library isn't initialized first by calling calltrace_init()
 * MEMORY OPERATION DESCRIPTIONS:
 *      1. It allocates heap memory using malloc if the internal string buffer isn't able to hold all the characters.
 * NOTE:
 *      1. It internally uses the two printf calls followed by puts(calltrace_string())
 * EXAMPLES:
 *      log_err("Somehthing bad happened with %s\n", "someone");
 *      //outputs [Error]: Something bad happened with someone
 *
 */
CALLTRACE_API function_signature(void, log_err, const char* format, ...);

/* log_wrn(const char* format, ...):
 * DESCRIPTION:
 *      Writes the formated string prefixed with "[Warning]: " and suffixed with calltrace_string()
 * PARAMS:
 *      1. format: format of the output string just like in printf
 *      2. (...) : variable arguements just like in printf
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, but it will crash the application with the library isn't initialized first by calling calltrace_init()
 * MEMORY OPERATION DESCRIPTIONS:
 *      1. It allocates heap memory using malloc if the internal string buffer isn't able to hold all the characters.
 * NOTE:
 *      1. It internally uses the two printf calls followed by puts(calltrace_string())
 * EXAMPLES:
 *      log_wrn("Somehthing vunerable with %s\n", "someone");
 *      //outputs [Warning]: Something vunerable with someone
 *
 */
CALLTRACE_API function_signature(void, log_wrn, const char* format, ...);

/* log_fetal_err(const char* format, ...):
 * DESCRIPTION:
 *      Writes the formated string prefixed with "[Fetal Error]: " and suffixed with calltrace_string()
 *      And terminates the application with return code = 0
 * PARAMS:
 *      1. format: format of the output string just like in printf
 *      2. (...) : variable arguements just like in printf
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, but it will crash the application with the library isn't initialized first by calling calltrace_init()
 * MEMORY OPERATION DESCRIPTIONS:
 *      1. It allocates heap memory using malloc if the internal string buffer isn't able to hold all the characters.
 * NOTE:
 *      1. It internally uses the two printf calls followed by puts(calltrace_string())
 * EXAMPLES:
 *      log_fetal_err("Somehthing very bad happened with %s\n", "someone");
 *      //outputs [Fetal Error]: Something very bad happened with someone
 *
 */
CALLTRACE_API function_signature(void, log_fetal_err, const char* format, ...);

/* log_msg(const char* format, ...):
 * DESCRIPTION:
 *      Writes the formated string prefixed with "[Log]: " and suffixed with calltrace_string()
 * PARAMS:
 *      1. format: format of the output string just like in printf
 *      2. (...) : variable arguements just like in printf
 * RETURNS:
 *      Nothing
 * ERROR DESCRIPTIONS:
 *      No Errors, but it will crash the application with the library isn't initialized first by calling calltrace_init()
 * MEMORY OPERATION DESCRIPTIONS:
 *      1. It allocates heap memory using malloc if the internal string buffer isn't able to hold all the characters.
 * NOTE:
 *      1. It internally uses the two printf calls followed by puts(calltrace_string())
 * EXAMPLES:
 *      log_msg("Object ID: %u\n", 12321413);
 *      //outputs [Log]: Object ID: 12321413
 *
 */
CALLTRACE_API function_signature(void, log_msg, const char* format, ...);
#	ifdef log_err
#		undef log_err
#   endif
#	ifdef log_wrn
#		undef log_wrn
#   endif
#	ifdef log_fetal_err
#		undef log_fetal_err
#   endif
#	ifdef log_msg
#		undef log_msg
#   endif
#   define log_err(...) define_alias_function_macro(log_err, __VA_ARGS__)
#   define log_wrn(...) define_alias_function_macro(log_wrn, __VA_ARGS__)
#   define log_fetal_err(...) define_alias_function_macro(log_fetal_err, __VA_ARGS__)
#   define log_msg(...) define_alias_function_macro(log_msg, __VA_ARGS__)
#else
#   define log_err(...)
#   define log_wrn(...)
#   define log_fetal_err(...)
#   define log_msg(...)
#endif

#ifdef __cplusplus
}
#endif

