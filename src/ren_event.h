#ifndef __REN_EVENT_H__
#define __REN_EVENT_H__

#include "list.h"

struct renon_dispatcher_t;
struct renon_min_heap_t;

typedef void (ren_event_func_t)(int, short, void *arg);

struct renon_event_t;

typedef struct renon_event_t {
    int fd;
    void *data;
    struct renon_dispatcher_t *dispatcher;
    renon_event_fun_t *callback;
    short events;
    int flags;
    struct list_head active;
    struct list_head event;
    unsigned int head_idx;
    struct timeval timeout;
    
}

struct renon_event_op_t {
    const char* name;
    void *(*init)(struct renon_dispatcher_t *);
    int (*add)(void *, struct renon_event_t *);
    int (*del)(void *, struct renon_event_t *);
    int (*poll)(struct renon_dispatcher_t *, void *, struct timeval *);
};

typedef struct renon_dispatcher_t {
    struct renon_event_op_t *op;
    void *data;

    struct list_head active_list;
    struct list_head event_list;
    struct renon_min_heap_t timeheap;
} renon_event_t;
} renon_dispatcher_t;

renon_dispatcher_t* renon_dispatcher_create();

int renon_dispatcher_run(renon_dispatcher_t *dispatcher);

void renon_event_init(renon_event_t *event, int fd, short events, renon_event_fun_t *callback, void *data);

int renon_event_add(renon_event_t *event, struct timeval, renon_dispatcher_t *dispatcher);

#endif /* __REN_EVENT_H__ */