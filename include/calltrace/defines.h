
#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

/*
	Switches:
		(NOTE: Automatically defined by the build system when building shared or static libraries)
		Development Switches:
			CALLTRACE_BUILD_STATIC_LIBRARY
			CALLTRACE_BUILD_DYNAMIC_LIBRARY
		(NOTE: Need to be defined by a dependent project before including any of this library's headers)
		UserCode Switches:
			CALLTRACE_USE_STATIC_LIBRARY
			CALLTRACE_USER_DYNAMIC_LIBRARY
*/

#if (defined _WIN32 || defined __CYGWIN__) && defined(__GNUC__)
#	define CALLTRACE_IMPORT_API __declspec(dllimport)
#	define CALLTRACE_EXPORT_API __declspec(dllexport)
#else
#	define CALLTRACE_IMPORT_API __attribute__((visibility("default")))
#	define CALLTRACE_EXPORT_API __attribute__((visibility("default")))
#endif

#ifdef CALLTRACE_BUILD_STATIC_LIBRARY
#	define CALLTRACE_API
#elif defined(CALLTRACE_BUILD_DYNAMIC_LIBRARY)
#	define CALLTRACE_API CALLTRACE_EXPORT_API
#elif defined(CALLTRACE_USE_DYNAMIC_LIBRARY)
#	define CALLTRACE_API CALLTRACE_IMPORT_API
#elif defined(CALLTRACE_USE_STATIC_LIBRARY)
#	define CALLTRACE_API
#else
#	define CALLTRACE_API
#endif

#if !defined(CALLTRACE_RELEASE) && !defined(CALLTRACE_DEBUG)
#	warning "None of CALLTRACE_RELEASE && CALLTRACE_DEBUG is defined; using CALLTRACE_DEBUG"
#	define CALLTRACE_DEBUG
#endif
