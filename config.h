#ifndef _CONFIG_H
#define _CONFIG_H

struct config_option {
    char *section;
    int section_len;
    char *key;
    int key_len;
    char *value;
    int value_len;
};

struct config {
    char **contents;
    int contents_count;
    struct config_option **options;
    int options_count;
};

struct config *init_config();
void config_read(struct config *conf, char *path);
struct config_option *config_get(struct config *conf, char *section, char *option);
void release_config(struct config **conf);

#endif
