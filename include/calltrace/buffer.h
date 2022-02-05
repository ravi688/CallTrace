
#pragma once

#include <calltrace/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define buffer_capacity(buffer) (*__buffer_capacity(buffer))
#define buffer_count(buffer) (*__buffer_count(buffer))
#define buffer_element_size(buffer) (*__buffer_element_size(buffer))

CALLTRACE_API u64* __buffer_capacity(void* buffer);
CALLTRACE_API u64* __buffer_count(void* buffer);
CALLTRACE_API u64* __buffer_element_size(void* buffer);

CALLTRACE_API void* buffer_ensure_capacity(void* buffer, u64 capacity);
CALLTRACE_API void* buffer_push(void* buffer, void* data);
CALLTRACE_API void* buffer_init();
CALLTRACE_API void buffer_clear(void* buffer);
CALLTRACE_API void buffer_destroy(void* buffer);
CALLTRACE_API void buffer_pop(void* buffer);


#ifdef __cplusplus
}
#endif
