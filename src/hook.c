#include "hook.h"
#include "util.h"
#include "format.h"

#define CODE_BUF 16

void st_hook_code(uc_engine *uc, uint64_t address, uint32_t size,
                  struct st_tracer *tracer) {
  uint8_t code_buf[CODE_BUF];
  int code_size = MIN(size, CODE_BUF);
  uc_mem_read(uc, address, code_buf, code_size);

  cs_insn *insn;
  int insn_count = cs_disasm(tracer->cs, code_buf, code_size, address, 1, &insn);
  if(insn_count) {
    printf("0x%llx: %s %s\n", insn->address, insn->mnemonic, insn->op_str);
  } else {
    printf("0x%llx: <invalid>\n", address);
  }
}

void st_hook_sys(uc_engine *uc, uint32_t intno, struct st_tracer *tracer) {
  if(intno != 0x80) {
    return;
  }

  struct st_syscall_args syscall_args;
  memset(&syscall_args, 0, sizeof(syscall_args));

  uc_reg_read(uc, UC_X86_REG_EAX, &syscall_args.syscall);

  uc_reg_read(uc, UC_X86_REG_EBX, &syscall_args.args[0]);
  uc_reg_read(uc, UC_X86_REG_ECX, &syscall_args.args[1]);
  uc_reg_read(uc, UC_X86_REG_EDX, &syscall_args.args[2]);
  uc_reg_read(uc, UC_X86_REG_ESI, &syscall_args.args[3]);
  uc_reg_read(uc, UC_X86_REG_EDI, &syscall_args.args[4]);

  switch(syscall_args.syscall) {
    case 0x1:
      uc_emu_stop(uc);
      break;
    case 0xb:
      uc_emu_stop(uc);
      break;
  }

  bstring formatted_syscall = st_format_syscall(&syscall_args, tracer);
  printf("syscall: %s\n", bdata(formatted_syscall));
  bdestroy(formatted_syscall);
}

