# SpinLock

## Naive Version

> `std::atomic_flag`
>
> - `clear()` => set value to `false`;
> - `test_and_set()` => try to be `true`;
> - Init `std::atomic_flag` with `ATOMIC_FLAG_INIT`

```c++
#include <atomic>
#include <thread>
#include <iostream>
// But this may not be good performance.

class naive_spinlock
{
private:
    std::atomic_flag m_af;
public:
    naive_spinlock():m_af(ATOMIC_FLAG_INIT){}
    void lock() noexcept 
    {
        while (m_af.test_and_set(std::memory_order_relaxed));
    }
    void unlock() noexcept 
    {
        m_af.clear(std::memory_order_relaxed);
    }
};

int x;
void add()
{
    static naive_spinlock lk;
    for (int i = 0; i < 10000000; ++i) {
        lk.lock();
        ++x;
        lk.unlock();
    }
}

int main()
{
    auto t1 = std::thread(add);
    auto t2 = std::thread(add);
    t1.join();
    t2.join();
    std::cout << x << '\n';
}
```

