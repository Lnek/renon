#include <stdio.h>
#include <stdlib.h>
#include "ren_malloc.h"

static void malloc_default_oom(size_t size) {
    fprintf(stderr, "malloc: Out of memory trying to allocate %zu bytes\n", size);
    fflush(stderr);
    abort();
}

static void (*ren_malloc_oom_handler)(size_t) = malloc_default_oom;

void *ren_malloc(size_t size) {
    void *p = malloc(size);
    
    if (!p) {
        ren_malloc_oom_handler(size);
    }

    return p;
}

void *ren_calloc(size_t size){

}

void *ren_realloc(void *p, size_t size) {

}

void ren_free(void *p) {
    free(p);
}
