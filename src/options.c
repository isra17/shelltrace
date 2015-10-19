#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "options.h"
#include "util.h"

#define BUF_SIZE 1024

const char *argp_program_version =
  "shelltrace 0.1";
const char *argp_program_bug_address =
  "isra17 <isra017@gmail.com>";

static char doc[] = "Shelltrace -- Utility to trace shellcode execution";

static char args_doc[] = "";

static struct argp_option options[] = {
  {"trace-code", 'c', 0, 0, "Trace instructions"},
  {"trace-sys", 's', 0, 0, "Trace syscalls"},
  {"shellcode", 'f', "FILE", 0, "File with shellcode"},
  {0}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct st_options* options = state->input;
  switch(key){
    case 'c':
      options->trace_code = 1;
      break;
    case 's':
      options->trace_syscall = 1;
      break;
    case 'f':
      {
        options->shellcode_size = readfile(
            arg,
            &options->shellcode);

        if(options->shellcode_size < 0) {
          perror("read_file_content(shellcode)");
        }
      }
      break;
  }
  return 0;
}

static struct argp st_argp = {options, parse_opt, args_doc, doc};

void st_options_init(struct st_options** poptions) {
  struct st_options* options;
  options = malloc(sizeof(struct st_options));
  options->trace_code = 0;
  options->trace_syscall = 1;

  options->shellcode_size = 0;
  options->shellcode = 0;

  options->shellcode_addr = 0x1000000;
  options->arch = UC_ARCH_X86;
  options->mode = UC_MODE_32;

  *poptions = options;
}

int st_options_parse(
    struct st_options* options,
    int argc,
    char** argv)
{
  return argp_parse(&st_argp, argc, argv, 0, 0, options);
}

void st_options_destroy(struct st_options* options) {
  if(options->shellcode) {
    free(options->shellcode);
  }

  free(options);
}

