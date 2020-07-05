

- -b` :: Byte.
- `-c` :: Char.
- `-f` :: File.
- `-d` :: Cut with delimiters.

#### Bytes

```bash
# Cut the first byte.
echo abcdefg | cut -b 1
# a
# -b :: Byte
# 1  :: Position

echo abcdefg | cut -b 1,3 # Cut 1st & 3rd char.
# ac

echo abcdefg | cut -b 1-3
# abc
```

### Characters

> When a character is of 2 bytes length, play with -b may give some strange output.
>
> So by using `-c` we can cut the characters based on our local character set.

```bash
echo 你好 | cut -c 1
# 你

echo 你好 | cut -b 1
# Some strange stuff.
```

#### Seq

```bash
echo $(seq -s ' ' 1 9)
# 1 2 3 4 5 6 7 8 9

echo $(seq -s ',' 1 9)
# 1,2,3,4,5,6,7,8,9,

echo $(seq 9)
# 1 2 3 4 5 6 7 8 9

echo {1..9}

for i in {1..9}; do echo {$i..9} ; done
# 1 2 3 4 5 6 7 8 9
# 2 3 4 5 6 7 8 9
# 3 4 5 6 7 8 9
# 4 5 6 7 8 9
# 5 6 7 8 9
# 6 7 8 9
# 7 8 9
# 8 9
# 9

# -s :: delimter string
for i in $(seq 10); do echo $(seq -s ' ' 1 9); done
```

### File

```bash
# cut [-d delim] [file ...]
cut -d ' ' -f 2 test.txt
```

#### Delimiter

```bash
echo 'ab cd ef gh' | cut -d ' ' -b 3
```

#### Tail

```bash
xxx | tail -n +2 
# Give you everything except the first line.
```

