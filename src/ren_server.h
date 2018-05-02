#ifndef __REN_SERVER_H__
#define __REN_SERVER_H__

struct renon_config_t;
struct renon_dispatcher_t;
struct renon_event_t;

typedef struct renon_server_t {
    int thread_num;
    struct renon_dispatcher_t *dispatcher;
    struct renon_event_t *event;
} renon_server_t;

renon_server_t *renon_server_create(struct renon_config_t *config);

#endif  /* __REN_SERVER_H__ */