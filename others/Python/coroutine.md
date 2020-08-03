## asyncio

### Non-Async

```python
def find_divisibles(inrange, div_by):
    print("finding nums in range {} divisible by {}".format(inrange, div_by))
    located = []
    for i in range(inrange):
        if i % div_by == 0:
            located.append(i)
    print("Done w/ nums in range {} divisible by {}".format(inrange, div_by))
    return located

def main():
    divs1 = find_divisibles(508000, 34113)
    divs2 = find_divisibles(100052, 3210)
    divs3 = find_divisibles(500, 3)

if __name__ == '__main__':
    main()
```

> 30.1 ms ± 295 µs per loop (mean ± std. dev. of 7 runs, 10 loops each)

### Async

```python
import asyncio


async def find_divisibles(inrange, div_by):
    print("finding nums in range {} divisible by {}".format(inrange, div_by))
    located = []
    for i in range(inrange):
        if i % div_by == 0:
            located.append(i)
    print("Done w/ nums in range {} divisible by {}".format(inrange, div_by))
    return located


async def main(loop):
    divs1 = loop.create_task(find_divisibles(508000, 34113))
    divs2 = loop.create_task(find_divisibles(100052, 3210))
    divs3 = loop.create_task(find_divisibles(500, 3))
    await asyncio.wait([divs1, divs2, divs3])


def run():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(loop))
    loop.close()
```

## Ref

- [Python Doc](https://docs.python.org/3/library/asyncio-task.html)