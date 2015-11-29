#include <stdio.h>
#include <string.h>
#include "config.h"

int main(int argc, char **argv) {
    struct config *conf = init_config();
    struct config_option *opt;

    config_read(conf, "./default.conf");
    config_read(conf, "./custom.conf");

    opt = config_get(conf, "dfs", "port");
    printf("dfs port: %.*s\n", opt->value_len, opt->value);

    opt = config_get(conf, "hive", "port");
    printf("hive port: %.*s\n", opt->value_len, opt->value);

    release_config(&conf);
    
    return 0;
}
