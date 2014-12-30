#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static void skip_whitespaces(char **ptr)  {
    while((' ' == **ptr || '\t' == **ptr || '\n' == **ptr) && 0x00 != **ptr) *ptr+=1;
}

static int parse_section(char *ptr) {
    int n = 0;
    while (']' != *ptr && 0x00 != *ptr) {
        n++;
        ptr++;
    }
    return n;
}

static int parse_option_key_n_skip(char **ptr) {
    int n=0, is_equal_found=0;
    while (' ' != **ptr && '\t' != **ptr && 0x00 != **ptr) {
        if ('=' == **ptr) {
            is_equal_found = 1;
            break;
        }

        n++;
        (*ptr)++;
    }

    skip_whitespaces(ptr);

    if(1 == is_equal_found) {
        (*ptr)++;
    }
    else {
        while ('=' != **ptr && 0x00 != **ptr) {
            (*ptr)++;
        }
        skip_whitespaces(ptr);
    }

    return n;
}

static int parse_option_value(char *ptr) {
    int n=0, whitespace_count=0;
    while (1) {
        whitespace_count = 0;

        while(' ' == *ptr || '\t' == *ptr) {
            whitespace_count += 1;
            ptr++;
        }

        if ('\n' == *ptr) {
            break;
        }

        n += whitespace_count;

        ptr++;
        n++;
    }
    return n;
}

static int config_get_return_id(struct config *conf, char *section, char *option) {
    int j;
    struct config_option* opt;
    for (j = 0; j < conf->options_count; j++) {
        opt = conf->options[j];

        if (strlen(section) != opt->section_len) {
            continue;
        }

        if (0 == strncmp(opt->section, section, opt->section_len)) {
            if (0 == strncmp(opt->key, option, opt->key_len)) {
                return j;
            }
        }
    }

    return -1;
}


struct config *init_config() {
    struct config *conf;
    conf = malloc(sizeof(struct config));
    memset(conf, 0x00, sizeof(struct config));
    return conf;
}

void config_read(struct config *conf, char *path) {
    FILE* fp; 
    long len;
    char *content, *ptr;
    int current_content_id, section_len=0, id;
    char* section = NULL;
    char tmp_section[512] = "", tmp_option[512] = "";
    struct config_option *opt = NULL;

    fp = fopen(path, "r");
    if (fp == NULL) {
        return;
    }   

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (len < 0) {
        return;
    }

    content = malloc(len + 1);
    memset(content, 0x00, len + 1);
    fread(content, len, 1, fp);

    conf->contents_count += 1;
    conf->contents = (char **)realloc(conf->contents, sizeof(char **) * conf->contents_count);
    current_content_id = conf->contents_count - 1;
    conf->contents[current_content_id] = content;

    ptr = content;

    while (0x00 != *ptr) {
        opt = (struct config_option*)malloc(sizeof(struct config_option));

        skip_whitespaces(&ptr);

        if (0x00 == *ptr) {
            break;
        }

        if ('[' == *ptr) {
            ptr++;//skip [
            section = ptr;
            section_len = parse_section(ptr);
            ptr += section_len;
            ptr++;//skip ]
            skip_whitespaces(&ptr);
        }

        opt->section = section;
        opt->section_len = section_len;

        opt->key = ptr;
        opt->key_len = parse_option_key_n_skip(&ptr);

        opt->value = ptr;
        opt->value_len = parse_option_value(ptr);

        ptr += opt->value_len;

        strncpy(tmp_section, section, section_len);
        tmp_section[section_len] = '\0';
        strncpy(tmp_option, opt->key, opt->key_len);
        tmp_option[opt->key_len] = '\0';

        id = config_get_return_id(conf, tmp_section, tmp_option);
        if (id < 0) {
            conf->options_count += 1;
            conf->options = (struct config_option**)realloc(conf->options, sizeof(struct config_option**) * conf->options_count);
            conf->options[conf->options_count-1] = opt;
        }
        else {
            free(conf->options[id]);
            conf->options[id] = opt;
        }

    }
}

struct config_option* config_get(struct config *conf, char *section, char *option) {
    struct config_option* opt;
    int j = config_get_return_id(conf, section, option);
    if (-1 == j) {
        return NULL;
    }
    return conf->options[j];
}

void release_config(struct config **conf) {
    int j;
    for (j = 0; j < (*conf)->contents_count; j++) {
        free((*conf)->options[j]);
        free((*conf)->contents[j]);
    }
    free(*conf);
    *conf = NULL;
}
