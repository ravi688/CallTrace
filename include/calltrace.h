
#pragma once

#ifdef GLOBAL_DEBUG
#define CALLTRACE_DEBUG
#endif

#ifdef GLOBAL_RELEASE
#define CALLTRACE_RELEASE
#endif

#if !defined(CALLTRACE_DEBUG) && !defined(CALLTRACE_RELEASE)
#	warning "None of CALLTRACE_DEBUG or CALLTRACE_RELEASE is defined; using CALLTRACE_DEBUG"
#	define CALLTRACE_DEBUG
#elif defined(CALLTRACE_DEBUG) && defined(CALLTRACE_RELEASE)
#	warning "Both of CALLTRACE_DEBUG and CALLTRACE_RELEASE are defined; using CALLTRACE_DEBUG"
#	define CALLTRACE_RELEASE
#endif

#ifdef CALLTRACE_DEBUG
#	define function_signature_void(return_type, function_name) return_type __##function_name(uint64_t __line__, const char* __function__, const char* __file__)
#	define function_signature(return_type, function_name, ...)  return_type __##function_name(__VA_ARGS__, uint64_t __line__, const char* __function__, const char* __file__)
#	define define_alias_function_void_macro(function_name) __##function_name(__LINE__, __FUNCTION__, __FILE__)
#	define define_alias_function_macro(function_name, ...) __##function_name(__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__)
#elif CALLTRACE_RELEASE
#	define function_signature_void(return_type, function_name) return_type function_name()
#	define function_signature(return_type, function_name, ...) return_type function_name(__VA_ARGS__)
#	define define_alias_function_macro(function_name, ...) function_name(__VA_ARGS__)
#	define define_alias_function_void_macro(function_name) function_name()
#endif