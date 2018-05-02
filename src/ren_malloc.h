#ifndef __REN_MALLOC_H__
#define __REN_MALLOC_H__

#define renon_alloc_type(T)        (T*)renon_malloc(sizeof(T))
#define renon_alloc_array(T, size) (T*)renon_malloc(sizeof(T) * size)

void *renon_malloc(size_t size);
void *renon_calloc(size_t size);
void *renon_realloc(void *p, siez_t size);
void renon_free(void *p);

#endif __REN_MALLOC_H__