#include "format.h"

static bstring strfmt(uint64_t arg, struct st_tracer *tracer);
static bstring astrfmt(uint64_t arg, struct st_tracer *tracer);
static bstring intfmt(uint64_t arg, struct st_tracer *tracer);

typedef bstring (*arg_formatter)(uint64_t, struct st_tracer *);

struct syscall_format {
  int syscall;
  char *name;
  arg_formatter arg_formatters[6];
};

struct syscall_format syscall_formats[] = {
    {1, "exit", {intfmt, 0}},
    {2, "fork", {intfmt, 0}},
    {3, "read", {intfmt, 0}},
    {4, "write", {intfmt, 0}},
    {5, "open", {intfmt, 0}},
    {6, "close", {intfmt, 0}},
    {7, "waitpid", {intfmt, 0}},
    {8, "creat", {intfmt, 0}},
    {9, "link", {intfmt, 0}},
    {10, "unlink", {intfmt, 0}},
    {11, "execve", {strfmt, astrfmt, astrfmt, 0}},
    {63, "dup2", {intfmt, intfmt, 0}},
    {102, "socketcall", {intfmt, intfmt, 0}},
};

struct syscall_format default_format = {
    -1, "<syscall(%d)>", {intfmt, intfmt, intfmt, intfmt, intfmt, 0}};

static struct syscall_format* find_syscall_formatter(int syscall) {
  int imin=0;
  int imax = sizeof(syscall_formats)/sizeof(syscall_formats[0]);

  while(imin <= imax) {
    int imid = (imin+imax)/2;
    if(syscall_formats[imid].syscall == syscall) {
      return &syscall_formats[imid];
    } else if(syscall_formats[imid].syscall < syscall) {
      imin = imid + 1;
    } else {
      imax = imid - 1;
    }
  }

  return &default_format;
}

bstring st_format_syscall(struct st_syscall_args *syscall_args,
                          struct st_tracer *tracer) {
  bstring formatted_syscall;
  struct syscall_format *formatter =
      find_syscall_formatter(syscall_args->syscall);

  formatted_syscall = bformat(formatter->name, syscall_args->syscall);

  bconchar(formatted_syscall, '(');
  arg_formatter* arg_formatter_it = formatter->arg_formatters;
  uint64_t *arg_it = syscall_args->args;
  while (*arg_formatter_it) {
    bstring arg_str = (*arg_formatter_it)(*arg_it, tracer);
    bconcat(formatted_syscall, arg_str);
    bdestroy(arg_str);

    arg_formatter_it++;
    arg_it++;
    if (*arg_formatter_it) {
      bcatcstr(formatted_syscall, ", ");
    }
  }
  bconchar(formatted_syscall, ')');

  return formatted_syscall;
}

bstring strfmt(uint64_t arg, struct st_tracer *tracer) {
  char mem_buf[256];
  uc_err err = uc_mem_read(tracer->uc, arg, mem_buf, sizeof(mem_buf));
  if(mem_buf[sizeof(mem_buf)-1]) {
    memcpy(mem_buf + sizeof(mem_buf) - 4, "...", 4);
  }

  if(err) {
    return bformat("0x%llx => <invalid string>");
  } else {
    return bformat("0x%llx => \"%s\"", arg, mem_buf);
  }
}

bstring astrfmt(uint64_t arg, struct st_tracer *tracer) {
  if(!arg) {
    return intfmt(arg, tracer);
  }

  bstring formatted_array = bformat("0x%llx => [", arg);
  uint64_t item = 0;

  // TODO: assumes 32 bits, make it arch/mode independant
  uc_mem_read(tracer->uc, arg, &item, 4);
  while(item) {
    bstring formatted_item = strfmt(item, tracer);
    bconcat(formatted_array, formatted_item);
    bdestroy(formatted_item);

    arg += 4;
    uc_mem_read(tracer->uc, arg, &item, 4);

    if(item) {
      bcatcstr(formatted_array, ", ");
    }
  }
  bconchar(formatted_array, ']');

  return formatted_array;
}

bstring intfmt(uint64_t arg, struct st_tracer *tracer) {
  bstring formatted_arg = bformat("0x%llx", arg);
  return formatted_arg;
}

