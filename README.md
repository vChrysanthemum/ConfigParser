`````
    struct config *conf = init_config();
    struct config_option *opt;
    char tmp_value[1024] = "";

    config_read(conf, "./default.conf");
    config_read(conf, "./custom.conf");

    opt = config_get(conf, "dfs", "port");
    strncpy(tmp_value, opt->value, opt->value_len);
    printf("%s\n", tmp_value);

    opt = config_get(conf, "hive", "port");
    strncpy(tmp_value, opt->value, opt->value_len);
    printf("%s\n", tmp_value);

    release_config(&conf);
    
    return 0;
```
