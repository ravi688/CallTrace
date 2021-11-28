
#pragma once

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
#elif defined(CALLTRACE_RELEASE)
#	define function_signature_void(return_type, function_name) return_type function_name()
#	define function_signature(return_type, function_name, ...) return_type function_name(__VA_ARGS__)
#	define define_alias_function_macro(function_name, ...) function_name(__VA_ARGS__)
#	define define_alias_function_void_macro(function_name) function_name()
#endif


#include <calltrace/defines.h>

typedef struct 
{
	u64 lineNo;
	const char* functionName;
	const char* fileName;
} callinfo_t;

void calltrace_init();
void calltrace_terminate();
void calltrace_buffer_push(callinfo_t info);
void calltrace_buffer_pop();
void calltrace_log();
const char* calltrace_string();

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
    function_signature(void, log_err, const char* format, ...);
    function_signature(void, log_wrn, const char* format, ...);
    function_signature(void, log_fetal_err, const char* format, ...);
    function_signature(void, log_msg, const char* format, ...);
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
