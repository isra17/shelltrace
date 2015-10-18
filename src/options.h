#ifndef ST_ARGP_H
#define ST_ARGP_H

#include <argp.h>

typedef struct st_options {
  int trace_code;
  int trace_syscall;
  char* shellcode;
  int shellcode_size;
} st_options_t;

void st_options_init(struct st_options* options);

int st_options_parse(
    int argc,
    char** argv,
    struct st_options* options);

#endif // ST_ARGP_H
