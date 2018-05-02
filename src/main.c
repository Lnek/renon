#include "ren_config.h"
#include "ren_event.h"
#include "ren_server.h"

int main(int argc, char *argv[]) {
    
    ren_config_t config;
    config.thread_num = 8;
    
    ren_server_t *server = ren_server_create(&config);
    while (ren_dispatcher_run(server->ren_dispatcher) > 0) {
        
    }
    
    return 0;
}