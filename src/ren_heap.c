#include "ren_event.h"
#include "ren_min_heap.h"

int min_heap_elem_greater(struct renon_event_t *a, struct renon_event_t *b) {
    return evutil_timercmp(&a->ev_timeout, &b->ev_timeout, >);
}

void renon_min_heap_ctor(renon_min_heap_t *s) {
    s->p = 0;
    s->n = 0;
    s->a = 0;
}

void renon_min_heap_dtor(renon_min_heap_t *s) {
    if (s->p) {
        free(s->p);
    }
}

void renon_min_heap_elem_init(struct renon_event_t *e) {
    e->min_heap_idx = -1;
}

int renon_min_heap_empty(renon_min_heap_t *s) {
    return 0u == s->n;
}

unsigned renon_min_heap_size(renon_min_heap_t *s) {
    return s->n;
}

struct renon_event_t* renon_min_heap_top(renon_min_heap_t *s) {
    return s->n ? *s->p : NULL;
}

int renon_min_heap_reserve(renon_min_heap_t *s, unsigned int n) {
    if (s->a < n) {
        struct renon_event_t **p;
        unsigned a = s->a ? a *2 : 8;
        if (a < n) {
            a = n;
        }
        p = (struct renon_t**)ren_realloc(s->p, a * sizeof (*p));
        if (!p) {
            return -1;
        }
        s->p = p;
        s->a = a;
    }
    return 0;
}

void min_heap_shift_up(renon_min_heap_t *s, unsigned hole_index, struct renon_event_t *e) {
    unsigned parent  = (hole_index - 1) / 2;
    while (hole_index && renon_min_heap_elem_greater(s->p[parent], e)) {
        (s->p[hole_index] = s->p[parent])->min_heap_idx = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    (s->p[hole_index] = e)->min_heap_idx = hole_index;
}

void min_heap_shift_down(renon_min_heap_t *s, unsigned hole_index, struct renon_event_t *e) {
    unsigned min_child = 2* (hole_index + 1);
    while(min_child <= s->n) {
        min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1]);
        if (!(min_heap_elem_greater(e, s->p[min_child]))) {
            break;
        }

        (s->p[hole_index] = s->p[min_child])->renon_min_heap_idx = hole_index;
        hole_index = min_child;
        min_child = 2 * (hole_index + 1);
    }
    min_heap_shift_up(s, hole_index, e);
}

int renon_min_heap_push(renon_min_heap_t *s, struct renon_event_t *e) {
    if (renon_min_heap_reserve(s, s->n + 1)) {
        return -1;
    }

    min_heap_shift_up(s, s->n++, e);
    return 0;
}

struct renon_event_t* renon_min_heap_pop(renon_min_heap_t *s) {
    if (s->n) {
        struct renon_event_t* e = *s->p;
        min_heap_shift_down(s, 0u, s->p[--s->n]);
        e->min_heap_idx = -1;
        return e;
    }
    return 0;
}

int renon_min_heap_erase(renon_min_heap_t* s, struct renon_event_t* e) {
    if (((unsigned int) -1 ) != e->min_heap_idx) {
        struct renon_event_t *last = s->p[--s->n];
        unsigned parent = (e->min_heap_idx - 1) / 2;
        if (e->min_heap_idx > 0 && min_heap_elem_greater(s->p[parent], last)) {
            min_heap_shift_up(s, e->min_heap_idx, last);
        } else {
            min_heap_shift_down(s, e->min_heap_idx, last);
        }
        e->min_heap_idx = -1;
        return 0;
    } 
    return -1;
}