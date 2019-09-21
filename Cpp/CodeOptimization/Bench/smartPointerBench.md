# SmartPointersInCpp



## Performance

> [Reference](<http://blog.davidecoppola.com/2016/10/performance-of-raw-pointers-vs-smart-pointers-in-cpp/>)

#### Accessing data

| **raw pointer** | **std::unique_ptr** | **std::shared_ptr** |
| :-------------- | :------------------ | :------------------ |
| 5               | 5                   | 5                   |

As expected there’s no notable difference during normal usage of any pointer.

#### Construction, data access and destruction

| **raw pointer** | **std::unique_ptr** | **std::shared_ptr** | **std::make_shared** |
| :-------------- | :------------------ | :------------------ | :------------------- |
| 23              | 23                  | **46**              | 27                   |

Things get more interesting when considering the whole life of pointers.

> 尽可能`std::make_shared`

#### Copy

| **raw pointer** | **std::shared_ptr** |
| :-------------- | :------------------ |
| 18              | 19                  |

## Rules

> [Ref](<https://www.modernescpp.com/index.php/c-core-guidelines-rules-to-smart-pointers>)

