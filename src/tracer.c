#include <stdlib.h>
#include <unicorn/unicorn.h>

#include "tracer.h"

#define ALIGN_ADDR(addr) ((addr) & ~0xFFF)
#define ALIGN_SIZE(addr, size) ((((addr) & 0xFFF) + (size) + 0xFFF) & ~0xFFF)

int st_tracer_init(struct st_tracer** ptracer, struct st_options* options) {
  struct st_tracer* tracer = malloc(sizeof(struct st_tracer));
  if(!tracer) {
    return -1;
  }

  tracer->options = options;

  // Create uc_engine
  tracer->last_uc_err = uc_open(options->arch, options->mode, &tracer->uc);
  if(tracer->last_uc_err) return -1;

  // Allocate shellcode
  tracer->last_uc_err = uc_mem_map(
      tracer->uc,
      ALIGN_ADDR(tracer->options->shellcode_addr),
      ALIGN_SIZE(tracer->options->shellcode_addr, tracer->options->shellcode_size),
      UC_PROT_ALL);
  if(tracer->last_uc_err) return -1;

  // Write shellcode
  tracer->last_uc_err = uc_mem_write(
      tracer->uc,
      tracer->options->shellcode_addr,
      tracer->options->shellcode,
      tracer->options->shellcode_size);
  if(tracer->last_uc_err) return -1;

  *ptracer = tracer;
  return 0;
}

int st_tracer_run(struct st_tracer* tracer) {
  tracer->last_uc_err = uc_emu_start(
      tracer->uc,
      tracer->options->shellcode_addr,
      tracer->options->shellcode_addr + tracer->options->shellcode_size + 1,
      0, 0);
  if(tracer->last_uc_err) return -1;

  return 0;
}

void st_tracer_destroy(struct st_tracer* tracer) {
  uc_close(tracer->uc);
  free(tracer);
}

