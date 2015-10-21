#ifndef ST_TRACER_H
#define ST_TRACER_H

#include <unicorn/unicorn.h>
#include <capstone/capstone.h>

#include "options.h"

typedef struct st_tracer {
  struct st_options* options;
  uc_engine* uc;
  int last_uc_err;
  uc_hook hooks[2];
  csh cs;
} st_tracer_t;

int st_tracer_init(struct st_tracer** tracer, struct st_options* options);

int st_tracer_run(struct st_tracer* tracer);

void st_tracer_destroy(struct st_tracer* tracer);

#endif // ST_TRACER_H
