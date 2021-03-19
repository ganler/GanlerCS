

## Use FIO to measure disk performance

> * https://dotlayer.com/how-to-use-fio-to-measure-disk-performance-in-linux/
> * https://fio.readthedocs.io/en/latest/

## Random Write

```shell
fio --name=randwrite --iodepth=16 --rw=randwrite --bs=4k --direct=1 --size=512M --numjobs=2 --runtime=240 --group_reporting
# --name: just a name, whatever;
# --iodepth: If the I/O engine is async, how large a queuing depth do we want to maintain?
# --runtime: max running time;
# --direct=1: non-buffered; To test real performance, you SHOULD always set it `1` to prevent PAGE CACHE optimization.
# --bs: block size; `a,b` means a for reads, and b for writes;
```

