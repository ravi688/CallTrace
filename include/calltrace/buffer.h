
#pragma once

#include <calltrace/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define buffer_capacity(buffer) (*__buffer_capacity(buffer))
#define buffer_count(buffer) (*__buffer_count(buffer))
#define buffer_element_size(buffer) (*__buffer_element_size(buffer))

u64* __buffer_capacity(void* buffer);
u64* __buffer_count(void* buffer);
u64* __buffer_element_size(void* buffer);

void* buffer_ensure_capacity(void* buffer, u64 capacity);
void* buffer_push(void* buffer, void* data);
void* buffer_init();
void buffer_clear(void* buffer);
void buffer_destroy(void* buffer);
void buffer_pop(void* buffer);


#ifdef __cplusplus
}
#endif
