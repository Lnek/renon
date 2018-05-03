#include "ren_config.h"
#include "ren_event.h"
#include "ren_server.h"

int main(int argc, char *argv[]) {
    
    renon_config_t config;
    config.thread_num = 8;
    
    renon_server_t *server = ren_server_create(&config);
    while (ren_dispatcher_run(server->dispatcher) > 0) {
        
    }
    
    return 0;
}