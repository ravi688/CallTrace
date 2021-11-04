
#pragma once

#define function_signature_void(return_type, function_name) return_type __##function_name(uint64_t __line__, const char* __function__, const char* __file__)
#define function_signature(return_type, function_name, ...)  return_type __##function_name(__VA_ARGS__, uint64_t __line__, const char* __function__, const char* __file__)
#define define_alias_function_void_macro(function_name) __##function_name(__LINE__, __FUNCTION__, __FILE__)
#define define_alias_function_macro(function_name, ...) __##function_name(__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__)
