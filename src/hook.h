#ifndef ST_HOOK_H
#define ST_HOOK_H

#include <unicorn/unicorn.h>

void st_hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);

void st_hook_sys(uc_engine *uc, uint32_t intno, void *user_data);

#endif
