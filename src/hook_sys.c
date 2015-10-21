#include "hook.h"

void sys_generic_handler(
    uc_engine* uc, uint64_t r1, uint64_t r2,
    uint64_t r3, uint64_t r4, uint64_t r5)
{

}

void sys_exit_handler(
    uc_engine* uc, uint64_t r1, uint64_t r2,
    uint64_t r3, uint64_t r4, uint64_t r5)
{

}

typedef void (*sys_handler)(uc_engine* uc, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

sys_handler handlers[] = {
  sys_exit_handler,
  sys_generic_handler
};

void st_hook_sys(uc_engine *uc, uint32_t intno, struct st_tracer *tracer) {
  if(intno != 0x80) {
    return;
  }

  uint64_t syscall = 0;
  uint64_t r1=0,r2=0,r3=0,r4=0,r5=0;
  uc_reg_read(uc, UC_X86_REG_EAX, &syscall);

  uc_reg_read(uc, UC_X86_REG_EBX, &r1);
  uc_reg_read(uc, UC_X86_REG_ECX, &r2);
  uc_reg_read(uc, UC_X86_REG_EDX, &r3);
  uc_reg_read(uc, UC_X86_REG_ESI, &r4);
  uc_reg_read(uc, UC_X86_REG_EDI, &r5);

  switch(syscall) {
    case 0x1:
      printf("syscall(%lld): exit(%llx)\n", syscall, r1);
      uc_emu_stop(uc);
      break;
    case 0xb:
      printf("syscall(%lld): execve(%llx, %llx, %llx)\n", syscall, r1, r2, r3);
      uc_emu_stop(uc);
      break;
    default:
      printf("syscall(%lld): (%llx, %llx, %llx)\n", syscall, r1, r2, r3);
  }
}

