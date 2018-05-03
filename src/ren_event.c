#include <stdio.h>
#include "ren_assert.h"
#include "ren_event.h"
#include "ren_malloc.h"
#include "ren_min_heap.h"

extern const struct renon_event_op_t epoll_ops;

renon_dispatcher_t* renon_dispatcher_create (int size) {
    renon_assert(size > 0);
    renon_dispatcher_t *dispatcher = renon_alloc_type(renon_dispatcher_t);
    dispatcher->op = &epoll_ops;
    dispatcher->data = dispatcher->op->init(dispatcher);
    INIT_LIST_HEAD(&(dispatcher->event_list));
    INIT_LIST_HEAD(&(dispatcher->active_list));
    renon_min_heap_ctor(&(dispatcher->time_heap));

    return dispatcher;
}

void renon_event_init(renon_event_t *event, int fd, short events, renon_event_func_t *callback, void *data) {
    event->fd = fd;
    event->callback = callback;
    event->data = data;
    event->events = events;
    INIT_LIST_HEAD(&(event->active_entry));
    INIT_LIST_HEAD(&(event->event_entry));
    renon_min_heap_elem_init(event);
}

static void event_queue_insert(renon_dispatcher_t *dispatcher, renon_event_t *event, int queue) {
    if (event->flags & queue) {
        /* Double insertion is possible for active events*/
        if (queue & RENON_EVENT_LIST_ACTIVE) {
            return;
        }
    }

    if (~event->flags & RENON_EVENT_LIST_INTERNAL) {
        base->event_count++;
    }

    event->flags |= queue;
    switch (queue) {
        case RENON_EVENT_LIST_INSERTED:
            list_add_tail(&(event->event_entry), &(dispatcher->event_list));
            break;
        case RENON_EVENT_LIST_ACTIVE:
            dispatcher->active_event_count++;
            list_add_tail(&(event->active_entry), &(dispatcher->active_list));
            break;
        case RENON_EVENT_LIST_TIMEOUT:
            renon_min_heap_push(&(dispatcher->time_heap), event);
            break;
        default:
            break;
    }
}

int renon_event_add(renon_event_t *event, struct timeval time, renon_dispatcher_t *dispatcher) {
    event->dispatcher = dispatcher;
    const renon_event_op_t *ops = dispatcher->op;
    int result = 0;

    renon_assert(!(event->flags & ~EVENT_LIST_ALL));

    if (time != NULL && !(event->flags & RENON_EVENT_LIST_TIMEOUT)) {
        if (renon_min_heap_reserve(&(dispatcher->time_heap), 1 + renon_min_heap_size(&dispatcher->time_heap)) == -1) {
            return (-1);
        }
    }

    if ((event->events & (RENON_EVENT_READ | RENON_EVENT_WRITE | RENON_EVENT_SIGNAL)) && 
        !(event->flags & (RENON_EVENT_LIST_INSERTED | RENON_EVENT_LIST_ACTIVE))) 
    {
        result = ops->add(evbase, ev);
        if (result != -1) {
            event_queue_insert(dispatcher, ev, RENON_EVENT_LIST_INSERTED);
        }
    }

    if (result != -1 && tv != NULL) {
        struct timeval now;

        if (ev->ev_flags & RENON_EVENT_LIST_TIMEOUT) {
            event_queue_remove(dispatcher, event, RENON_EVENT_LIST_TIMEOUT);
        }

        if ((event->flags & RENON_EVENT_LIST_ACTIVE) && (event->result & EV_TIMEOUT)) {
            event_queue_remove(dispatcher, event, RENON_EVENT_LIST_ACTIVE);
        }

        gettime(dispatcher, &now);
        evutil_timeradd(&now, tv, &ev->ev_timeout);

        event_queue_insert(dispatcher, ev, RENON_EVENT_LIST_TIMEOUT);
    }
    return result;
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

void renon_event_active(renon_event_t *event, int result, short ncalls) {
    if (event->flags & RENON_EVENT_LIST_ACTIVE) {
        event->result |= result;
        return;
    }

    event->result = result;
    event->ncalls = ncalls;
    event_queue_insert(event->dispatcher, event, RENON_EVENT_LIST_ACTIVE);
}