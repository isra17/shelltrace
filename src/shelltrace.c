#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "tracer.h"

int main(int argc, char* argv[]) {
  struct st_options* options;
  struct st_tracer* tracer;

  st_options_init(&options);
  st_options_parse(options, argc, argv);

  if(st_tracer_init(&tracer, options)) {
    if (tracer) {
      fprintf(stderr, "Error: Initialization failed: %d\n", tracer->last_uc_err);
    } else {
      fprintf(stderr, "Error: Cannot allocate tracer\n");
    }
  }

  if(st_tracer_run(tracer)) {
    fprintf(stderr, "Error: Running shellcore failed: %d\n", tracer->last_uc_err);
  }

  st_tracer_destroy(tracer);
  st_options_destroy(options);

  return 0;
}
