# GDB Review

> Other tools:
>
> - Performance:
>   - gprof

https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/gdb.html

## To Use GDB

- `g++ -g ${OTHERS}`
- `gdb ${EXE}`

Debug core file.

- `gdb ${EXE} ${CORE_DUMP_FILE}`

## CMDs

### Running

- `run|r`
- `continue|c`: go to next ckp.
- `next|n`: Single step.
- `until`: Run until finish current loop.
- `until`: Run until xx line.
- `call`: Call functions.
- `quit|q`: Exit.

### CheckPoint

- `break n | b n`：Set a ckp in line n;

### Print

- `print|p ${var}`
- `info locals`：Query current stack vars.

### Query

- `where/bt`: Current running stack.
- `bt backtrace`: Current call stack.
- `show args`
- `set args ${args}`

### Debug Segmentation Fault

- `r`
- `bt` (backtrace)