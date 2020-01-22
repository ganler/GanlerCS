#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <thread>

// #define TESTIT

template <typename ... Args>
void setpixel(Args&& ... args)
{
#ifdef TESTIT
    ((std::cout << args << ' '), ...);
#endif
}

void bresenham(int x0, int y0, int x1, int y1)
{// Modified from https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (setpixel(x0, y0), x0 != x1 || y0 != y1)
    {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}

void jiawei(int x0, int y0, int x1, int y1)
{
    constexpr auto to_fixp   = [](int val){ return val << 8; };
    constexpr auto from_fixp = [](int val){ return val >> 8; };

    const int dx0_1 = x1 - x0, dy0_1 = y1 - y0;
    const int base = std::max(std::abs(dx0_1), std::abs(dy0_1));
    const int dx = to_fixp(dx0_1) / base, dy = to_fixp(dy0_1) / base;

    x0 = to_fixp(x0); x1 = to_fixp(x1);
    y0 = to_fixp(y0); y1 = to_fixp(y1);

    while(setpixel(from_fixp(x0), from_fixp(y0)), x0 != x1 && y0 != y1)
        x0 += dx, y0 += dy;
}

template <typename V, typename F>
void test(V&& v, F&& f)
{
    for(std::size_t i = 0; i < v.size() / 4; ++i)
    {
        f(v[i], v[i+1], v[i+2], v[i+3]);
#ifdef TESTIT
    std::cout << '\n';
#endif
    }
}

std::vector<int> dataset(4096);

const auto init_ = [](){
    std::random_device rd;
    std::default_random_engine e{rd()};
    std::uniform_int_distribution<int> u{-1024, 1024};
    std::generate(dataset.begin(), dataset.end(), [&](){return u(e);});
    return nullptr;
}();

#ifdef TESTIT
int main()
{
    test(dataset, bresenham);
    test(dataset, jiawei);
}
#endif

static void bresenham_(benchmark::State& state) {
  for (auto _ : state) {
    test(dataset, bresenham);
  }
}
// Register the function as a benchmark
BENCHMARK(bresenham_);

static void jiawei_(benchmark::State& state) {
  for (auto _ : state) {
    test(dataset, jiawei);
  }
}
BENCHMARK(jiawei_);
