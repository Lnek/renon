#ifndef __REN_EVENT_H__
#define __REN_EVENT_H__

#include <sys/time.h>
#include "list.h"
#include "ren_min_heap.h"

#define RENON_EVENT_LIST_TIMEOUT  0x01
#define RENON_EVENT_LIST_INSERTED 0x02
#define RENON_EVENT_LIST_SIGNAL   0x04
#define RENON_EVENT_LIST_ACTIVE   0x08
#define RENON_EVENT_LIST_INTERNAL 0x10
#define RENON_EVENT_LIST_INIT     0x80

#define RENON_EVENT_LIST_ALL      (0xf000 | 0x9f)

#define RENON_EVENT_TIMEOUT 0x01
#define RENON_EVENT_READ    0x02
#define RENON_EVENT_WRITE   0x04
#define RENON_EVENT_SIGNAL  0x08
#define RENON_EVENT_PERIST  0x10  /* Persistant event */

struct renon_dispatcher_t;
struct renon_event_t;
typedef int (renon_event_func_t)(int fd, short flag, void *arg);

typedef struct renon_event_t {
    int fd;
    void *data;
    struct renon_dispatcher_t *dispatcher;
    renon_event_fun_t *callback;
    short events;
    int flags;
    int result;
    struct list_head active_entry; /* for active list */
    struct list_head event_entry;  /*for dispatcher event list */
    unsigned int head_idx;
    struct timeval timeout;
    
} renon_event_t;

typedef struct renon_event_op_t {
    const char* name;
    void *(*init)(struct renon_dispatcher_t *);
    int (*add)(void *, struct renon_event_t *);
    int (*del)(void *, struct renon_event_t *);
    int (*poll)(struct renon_dispatcher_t *, void *, struct timeval *);
} ren_event_op_t;

typedef struct renon_dispatcher_t {
    const struct renon_event_op_t *op;
    void *data;

    unsigned int event_count;
    unsigned int active_event_count;
    struct list_head active_list; /* for active events */
    struct list_head event_list;  /*for manage events */
    struct renon_min_heap_t time_heap;
} renon_dispatcher_t;

renon_dispatcher_t* renon_dispatcher_create();

int renon_dispatcher_run(renon_dispatcher_t *dispatcher);

void renon_event_init(renon_event_t *event, int fd, short events, renon_event_fun_t *callback, void *data);

int renon_event_add(renon_event_t *event, struct timeval, renon_dispatcher_t *dispatcher);

void renon_event_active(renon_event_t *event, int result, short ncalls);

#endif /* __REN_EVENT_H__ */