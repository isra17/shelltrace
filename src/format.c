#include "format.h"

static bstring string_arg_formatter(uint64_t arg, struct st_tracer *tracer);
static bstring int_arg_formatter(uint64_t arg, struct st_tracer *tracer);

typedef bstring (*arg_formatter)(uint64_t, struct st_tracer *);

struct syscall_format {
  char *name;
  arg_formatter arg_formatters[6];
};

struct syscall_format syscall_formats[] = {
    {"<invalid(0)>", {0}},
    {"exit", {int_arg_formatter, 0}},
    {"fork", {int_arg_formatter, 0}},
    {"read", {int_arg_formatter, 0}},
    {"write", {int_arg_formatter, 0}},
    {"open", {int_arg_formatter, 0}},
    {"close", {int_arg_formatter, 0}},
    {"waitpid", {int_arg_formatter, 0}},
    {"creat", {int_arg_formatter, 0}},
    {"link", {int_arg_formatter, 0}},
    {"unlink", {int_arg_formatter, 0}},
    {"execve", {int_arg_formatter, 0}}
};

struct syscall_format default_format = {"<syscall(%d)>",
                                        {int_arg_formatter, int_arg_formatter,
                                         int_arg_formatter, int_arg_formatter,
                                         int_arg_formatter, 0}};

bstring st_format_syscall(struct st_syscall_args *syscall_args,
                          struct st_tracer *tracer) {
  bstring formatted_syscall;
  struct syscall_format *formatter;

  if (syscall_args->syscall <
      sizeof(syscall_formats) / sizeof(syscall_formats[0])) {
    formatter = &syscall_formats[syscall_args->syscall];
    formatted_syscall = bfromcstr(formatter->name);
  } else {
    formatter = &default_format;
    formatted_syscall = bformat(formatter->name, syscall_args->syscall);
  }

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

bstring string_arg_formatter(uint64_t arg, struct st_tracer *tracer) {
  return bfromcstr("<string>");
}

bstring int_arg_formatter(uint64_t arg, struct st_tracer *tracer) {
  bstring formatted_arg = bformat("0x%llx", arg);
  return formatted_arg;
}

