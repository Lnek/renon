#ifndef __REN_HEAP_H__
#define __REN_HEAP_H__

struct renon_event_t;

typedef struct renon_min_heap_t {
    struct renon_event_t **top;
    unsigned int n;
    unsigned int a;
} renon_head_t;

void renon_min_heap_ctor(renon_min_heap_t* s);
void renon_min_heap_dtor(renon_min_heap_t* s);
void renon_heap_elem_init(struct renon_event_t* e);
int renon_min_heap_empty(renon_min_heap_t* s);
unsigned renon_min_heap_size(renon_min_heap_t* s);
struct renon_event_t* renon_min_heap_top(renon_min_heap_t* s);
int renon_min_heap_reserve(renon_min_heap_t* s, unsigned n);
int renon_min_heap_push(renon_min_heap_t* s, struct renon_event_t* e);
struct renon_event_t* renon_min_heap_pop(renon_min_heap_t* s);
int renon_min_heap_erase(renon_min_heap_t* s, struct renon_event_t* e);

#endif  /* __REN_HEAP_H__ */