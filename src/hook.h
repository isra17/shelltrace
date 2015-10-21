#ifndef ST_HOOK_H
#define ST_HOOK_H

#include <unicorn/unicorn.h>
#include "tracer.h"

void st_hook_code(uc_engine *uc, uint64_t address, uint32_t size,
                 struct st_tracer  *tracer);

void st_hook_sys(uc_engine *uc, uint32_t intno, struct st_tracer *tracer);

#endif
