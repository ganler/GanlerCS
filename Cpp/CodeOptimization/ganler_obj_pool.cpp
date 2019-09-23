#include <memory>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <future>

namespace ganler
{
template <typename T>
class obj_pool
{
public:
    using uptr_t = std::unique_ptr<T, std::function<void(T*)>>;
    static inline bool empty()
    {
        return size_.load(std::memory_order_acquire) == 0;
    }
    static inline std::size_t size()
    {
        return size_.load(std::memory_order_acquire);
    }
    static inline void recycle(std::unique_ptr<T> ptr)
    {
        {
            std::lock_guard l(mu_);
            pool_.push_back(std::move(ptr));
        }
        size_.fetch_add(1, std::memory_order_acq_rel);
    }
    template <typename ... Args>
    static inline uptr_t build(Args&& ... args)
    {
        return uptr_t(new T(std::forward<Args>(args)...), deleter_);
    }
    static inline uptr_t get()
    { // please check `empty` by urself :) or go and fuck lrz.
        uptr_t ret;
        {
            std::lock_guard l(mu_);
            ret = uptr_t(pool_.back().release(), deleter_);
            pool_.pop_back();
        }
        size_.fetch_sub(1, std::memory_order_acq_rel);
        return ret; // auto move.
    }
private:
    static inline std::deque<std::unique_ptr<T>> pool_;
    static inline std::mutex                     mu_;
    static inline std::atomic<std::size_t>       size_;
    static inline std::function<void(T*)>        deleter_ = [](T* r){
        {
            std::lock_guard l(mu_);
            pool_.emplace_back(r);
        }
        size_.fetch_add(1, std::memory_order_acq_rel);
    };
};
}

void tester()
{
    using namespace ganler;
    std::cout << std::this_thread::get_id() << '\n';
    obj_pool<int>::uptr_t p;
    {
        obj_pool<int> fuckpool;
        {
            auto lrznb = fuckpool.build(666);
            std::cout << *lrznb << '\n';
            std::cout << fuckpool.size() << '\n';
        }
        p = fuckpool.get();
        std::cout << *p << '\n';
        std::cout << fuckpool.size() << '\n';
    }
    std::cout << *p << '\n';
}

int main()
{
    std::initializer_list<decltype(std::async(std::launch::async, tester))> lrz_fker{
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester),
            std::async(std::launch::async, tester)};
}