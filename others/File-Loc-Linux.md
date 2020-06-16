```bash
# From path.
which ${NAME}

# -b => binary | -m => man
whereis ${NAME}
whereis stdio.h

# built-in alias function ...
type ${NAME}

# Find the path of files from the system's database. (Usually updated once a day. Deleted files may also be found.)
locate ${NAME}    # 模糊匹配
locate -e ${NAME} # Only locate existing files.
locate -c ${NAME} # Count
locate -i ${NAME} # Ignore CASE/case
locate -r what$   # End with 'what'.

# Find
find ${WHERE} -name "sort*"
find ./ -size 1k  #查找当前目录下小于1k的文件
./test
./sort4.txt
./sort2.txt
./sort3.txt
./test.sh
./sort.txt
find -size +1M  #查找当前目录下大于1M的文件
./test.zip
```

https://www.yanbinghu.com/2018/11/24/18359.html