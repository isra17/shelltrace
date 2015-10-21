#include <stdlib.h>
#include <unicorn/unicorn.h>

#include "tracer.h"
#include "hook.h"

#define ALIGN_ADDR(addr) ((addr) & ~0xFFF)
#define ALIGN_SIZE(addr, size) ((((addr) & 0xFFF) + (size) + 0xFFF) & ~0xFFF)

int st_tracer_init(struct st_tracer** ptracer, struct st_options* options) {
  struct st_tracer* tracer = malloc(sizeof(struct st_tracer));
  *ptracer = tracer;

  if(!tracer) {
    return -1;
  }

  tracer->options = options;
  for(size_t i=0; i < 2; i++) {
    tracer->hooks[i] = 0;
  }

  // Create uc_engine
  tracer->last_uc_err = uc_open(options->arch, options->mode, &tracer->uc);
  if(tracer->last_uc_err) return -1;

  // Allocate stack
  int stack_mem_start = tracer->options->stack_addr - tracer->options->stack_size;
  tracer->last_uc_err = uc_mem_map(
      tracer->uc,
      ALIGN_ADDR(stack_mem_start),
      ALIGN_SIZE(stack_mem_start, tracer->options->stack_size),
      UC_PROT_READ | UC_PROT_WRITE);
  if(tracer->last_uc_err) return -1;

  uc_reg_write(tracer->uc, UC_X86_REG_ESP, &tracer->options->stack_addr);

  // Allocate shellcode
  tracer->last_uc_err = uc_mem_map(
      tracer->uc,
      ALIGN_ADDR(tracer->options->shellcode_addr),
      ALIGN_SIZE(tracer->options->shellcode_addr, tracer->options->shellcode_size),
      UC_PROT_ALL);
  if(tracer->last_uc_err) return -1;

  tracer->last_uc_err = uc_mem_write(
      tracer->uc,
      tracer->options->shellcode_addr,
      tracer->options->shellcode,
      tracer->options->shellcode_size);
  if(tracer->last_uc_err) return -1;

  // Add hooks
  if(tracer->options->trace_code) {
    uc_hook_add(
        tracer->uc,
        &tracer->hooks[0],
        UC_HOOK_CODE,
        st_hook_code,
        tracer,
        1, 0);
  }

  if(tracer->options->trace_syscall) {
    uc_hook_add(
        tracer->uc,
        &tracer->hooks[1],
        UC_HOOK_INTR,
        st_hook_sys,
        tracer,
        1, 0);
  }

  // Init capstone
  // TODO: Translate uc arch to cs
  if (cs_open(CS_ARCH_X86, CS_MODE_32, &tracer->cs) != CS_ERR_OK) {
    // TODO: expose the error somewhere in `tracer`
    return -1;
  }

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
  cs_close(&tracer->cs);
  uc_close(tracer->uc);
  free(tracer);
}

