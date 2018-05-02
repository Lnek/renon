#include "ren_assert.h"
#include "ren_config.h"
#include "ren_event.h"
#include "ren_malloc.h"
#include "ren_net.h"
#include "ren_server.h"

static int init_server_event(struct ren_server_t *server) {

    int fd = ren_net_create_listener(9999, "127.0.0.1");
    
    if (fd < 0) {
        return -1;
    }
}


ren_server_t *ren_server_create(struct ren_config_t *config) {

    ren_assert(config);
    ren_assert(config->thread_num > 0);
    
    ren_server_t *server = renon_alloc_type(ren_server_t);
    server->thread_num = config->thread_num;
    server->dispatcher = renon_alloc_type(ren_dispatcher_t);
    server->event = renon_alloc_type(ren_io_event_t);

    if (init_server_event(server) < 0) {
        renon_free(server->event);
        renon_free(server->dispatcher);
        renon_free(server);
        return NULL;
    }

    return server;
}