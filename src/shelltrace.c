#include <stdlib.h>
#include <argp.h>

#include "options.h"

int main(int argc, char* argv[]) {
  struct st_options options;
  st_options_init(&options);
  st_options_parse(argc, argv, &options);
  return 0;
}
