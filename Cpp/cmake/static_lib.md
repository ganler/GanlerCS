We have:

- main.cpp
- func.cpp

And we want to make `func.cpp` a static library.

## Executable files

Then we can write:

```cmake
add_executable(main main.cpp)               # 1st CLION executable target
add_executable(main_func main.cpp func.cpp) # 2nd CLION executable target
```

## Static libs

Let's creat a static library for `func.cpp`.

```cmake
add_library(func STATIC func.cpp)
# If we run `build` in CLION, we'll get a `libfunc.a` in build folder.
```

## Linking Static libs

Supposing that:

- `app/lib/libfunc.a`

```cmake
## find_library(<VAR> {file name} {PATH})
find_library(LIBFUNC func lib)
target_link_libraries(APP LINK_PUBLIC ${LIBFUNC})
```

**Make sure that `add_executable` is before `target_link_libraries`.**

(We should do linking after building)

## Good sources

https://www.jetbrains.com/help/clion/managing-cmake-project-files.html

https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html