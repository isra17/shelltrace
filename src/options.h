#ifndef ST_ARGP_H
#define ST_ARGP_H

#include <argp.h>
#include <unicorn/unicorn.h>

#include "stdint.h"

typedef struct st_options {
  int trace_code;
  int trace_syscall;

  char* shellcode;
  int shellcode_size;

  uintptr_t shellcode_addr;
  uc_arch arch;
  uc_mode mode;
} st_options_t;

void st_options_init(struct st_options** options);

int st_options_parse(
    struct st_options* options,
    int argc,
    char** argv);

void st_options_destroy(struct st_options* options);

#endif // ST_ARGP_H
