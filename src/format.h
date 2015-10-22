#ifndef ST_FORMATTER_H
#define ST_FORMATTER_H

#include <stdint.h>
#include "tracer.h"
#include "bstrlib.h"

struct st_syscall_args {
  uint64_t syscall;
  uint64_t args[5];
};

bstring st_format_syscall(struct st_syscall_args *syscall_args,
                        struct st_tracer *tracer);

#endif

