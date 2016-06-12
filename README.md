# Shelltrace
Shelltrace is build with [Unicorn](https://github.com/unicorn-engine/unicorn)
and is meant to trace instruction and syscall of random shellcodes.

## Usage
Trace a shellcode from stdin:
```
$ shelltrace -f sh.bin
```

Trace ROP chain with multiples loaded files:
```
$ shelltrace -e bin.elf:0x400000 -e libc.so:0x7f000000 -r -- < rop.bin
```

List all options
```
$ shelltrace --help
```
