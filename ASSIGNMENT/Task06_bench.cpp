// Q6: cost of friend / member / getter functions, with and without inlining.
// Build twice:  g++ -std=c++17 -O0 Q6_bench.cpp   and   g++ -std=c++17 -O2 Q6_bench.cpp
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#define NOINLINE __attribute__((noinline))

class Time {
    int h, m;
public:
    Time(int hh = 0, int mm = 0) : h(hh), m(mm) {}
    int getH() const { return h; }                         // getters
    int getM() const { return m; }
    int minutesTo(const Time& b) const { return std::abs((b.h * 60 + b.m) - (h * 60 + m)); }       // member, inlineable
    NOINLINE int minutesToNoInline(const Time& b) const { return std::abs((b.h * 60 + b.m) - (h * 60 + m)); }
    friend int minutesBetween(const Time& a, const Time& b);
    friend int minutesBetweenNoInline(const Time& a, const Time& b);
};
inline int minutesBetween(const Time& a, const Time& b) { return std::abs((b.h * 60 + b.m) - (a.h * 60 + a.m)); }   // friend, inline
NOINLINE int minutesBetweenNoInline(const Time& a, const Time& b) { return std::abs((b.h * 60 + b.m) - (a.h * 60 + a.m)); }
inline int minutesByGetters(const Time& a, const Time& b) { return std::abs((b.getH() * 60 + b.getM()) - (a.getH() * 60 + a.getM())); }

volatile long long sink;
template <class F> double run(const std::vector<Time>& v, long long calls, F f)
{
    auto t0 = std::chrono::steady_clock::now();
    long long s = 0; const std::size_t mask = v.size() - 1;
    for (long long i = 0; i < calls; ++i) s += f(v[i & mask], v[(i * 7 + 3) & mask]);
    sink = s;
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
}
int main()
{
    std::mt19937 g(1); std::vector<Time> v(1024);
    for (auto& t : v) t = Time(g() % 24, g() % 60);
    const long long N = 10000000;
#ifdef __OPTIMIZE__
    std::cout << "optimised build (-O2)\n";
#else
    std::cout << "unoptimised build (-O0)\n";
#endif
    auto best = [&](auto f) { double b = 1e18; for (int r = 0; r < 5; ++r) b = std::min(b, run(v, N, f)); return b; };
    std::cout << "10,000,000 calls, best of 5 (ms)\n";
    std::cout << "friend, inline            : " << best([](const Time& a, const Time& b) { return minutesBetween(a, b); }) << '\n';
    std::cout << "friend, noinline          : " << best([](const Time& a, const Time& b) { return minutesBetweenNoInline(a, b); }) << '\n';
    std::cout << "member, inline            : " << best([](const Time& a, const Time& b) { return a.minutesTo(b); }) << '\n';
    std::cout << "member, noinline          : " << best([](const Time& a, const Time& b) { return a.minutesToNoInline(b); }) << '\n';
    std::cout << "getters (free function)   : " << best([](const Time& a, const Time& b) { return minutesByGetters(a, b); }) << '\n';
}
