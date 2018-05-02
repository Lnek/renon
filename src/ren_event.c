#include <stdio.h>
#include "ren_assert.h"
#include "ren_event.h"
#include "ren_malloc.h"

extern const struct renon_event_op_t epoll_ops;

renon_dispatcher_t* renon_dispatcher_create (int size) {
    renon_assert(size > 0);
    renon_dispatcher_t *dispatcher = renon_alloc_type(renon_dispatcher_t);
    dispatcher->op = &epoll_ops;
    dispatcher->data = dispatcher->op->init(dispatcher);
    INIT_LIST_HEAD(&(dispatcher->event_list));
    INIT_LIST_HEAD(&(dispatcher->active_list));
    renon_min_heap_ctor(&(dispatcher->timeheap));

    return dispatcher;
}

void renon_event_init(renon_event_t *event, int fd, short events, renon_event_func_t *callback, void *data) {
    event->fd = fd;
    event->callback = callback;
    event->data = data;
    event->events = events;
    INIT_LIST_HEAD(&(event->active_list));
    INIT_LIST_HEAD(&(event->event_node));
    renon_min_heap_elem_init(event);
}

int renon_event_add(renon_event_t *event, struct timeval time, renon_dispatcher_t *dispatcher) {
    event->dispatcher = dispatcher;
    renon_event_op_t *ops = dispatcher->op;
    list_add_tail(&(event->), &dispatcher->event_list);
}

int renon_dispatcher_run(renon_dispatcher_t *dispatcher) {
    const struct renon_event_op_t *op = dispatcher->op;
    void *base = dispatcher->;
    struct timeval tv;
    struct timeval *tv_p;
    int res, done;

    while (!done) {
        res = op->dispatch(dispatch, base, tv_p);

        if (res == -1) {
            return -1;
        }
    }

    return 0;
}