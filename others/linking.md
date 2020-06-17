## Object File

- **Linux**: ELF: Executable Linkable Format
- **Windows**: PE: Portable Executable

Generally, there're 3 formats of object file:

1. **EXEC**: Executable File
2. **REL**: Relocatable File
3. **DYN**: Share Object File

```bash
readelf -h ${BINARY}
```

## Static Lib

REL file packes itself as a stand-alone file using some magic tricks. And it can become EXEC file when linked.

When doing linking, this "stand-alone" file will **copy** the content it needs from itself to the EXEC file. This stand-alone file is called static lib. Usually ended with `.a` which means `archive`.

```bash
gcc -c main.c # Now we got the main.o REL file. It is still not executable.
# -c means compiling with out linking. This will only generate an object file.
gcc -static -o main main.o -lm
# -static means this is linking to a static lib.
# -lm : linking the math lib.

# Say that main.c utilized the `exp` function. The `exp` function will be copied into main(EXEC) in the linking stage.
```

- Note that the order matters. It will start scanning from left to right. (main.o first and then libm.a)

## Shared Lib

Unlike static lib, binary codes will not be copied into executatble files when linking shared lib. The linking stage is performed in the runtime. This shared libs usually ends up with `.so`(shared obj.)

To do static linking, we usually add the flag `-static`. And `dynamic linking` is regarded as  the default linking method which means we can just type `-lxxx` when we prefer dynamic linking.

## Difference

- Size
- Compatibility: Once the function impl. changed in the static lib you are using, you have to recompile to update.
- When linking dynamic lib, the dynamic lib must exist.