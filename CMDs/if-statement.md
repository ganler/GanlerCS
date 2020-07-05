## Non if way

```bash
echo 1
echo 2
# equal to
echo 1; echo 2
# different from
echo 1 && echo 2
# The 2nd command will only be executed if the 1st command succeeds.
```

## If way

```bash
if [ 100 -eq 100 ] ; then
	echo "YES"
else
	echo "NO"
fi
```

