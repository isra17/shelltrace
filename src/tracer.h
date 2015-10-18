#ifndef ST_TRACER_H
#define ST_TRACER_H

#include "options.h"

typedef struct st_tracer {
  struct st_options* options;
  uc_engine* uc;
  int last_uc_err;
} st_tracer_t;

int st_tracer_init(struct st_tracer** tracer, struct st_options* options);

int st_tracer_run(struct st_tracer* tracer);

void st_tracer_destroy(struct st_tracer* tracer);

#endif // ST_TRACER_H
