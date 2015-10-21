#include "hook.h"
#include "util.h"

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

