// Prefix vs postfix; checked vs unchecked access.  Compile -O0 and -O2
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
struct Time { int secs = 0;
    Time& operator++() { secs = (secs + 1) % 86400; return *this; }
    Time operator++(int) { Time old = *this; ++*this; return old; } };
struct Big { int secs = 0; char pad[256]; std::string name = "a long enough string to avoid SSO so copy costs something real";
    Big& operator++() { ++secs; return *this; }
    Big operator++(int) { Big old = *this; ++*this; return old; } };
template <class F> double ms(F f) { auto t0 = std::chrono::steady_clock::now(); f(); return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count(); }
int main() {
    const int N = 10000000; volatile int sink = 0;
    Time t1; double pre = ms([&] { for (int i = 0; i < N; ++i) ++t1; });  sink = t1.secs;
    Time t2; double post = ms([&] { for (int i = 0; i < N; ++i) t2++; }); sink = t2.secs;
    std::cout << "Time  : prefix " << pre << " ms, postfix " << post << " ms\n";
    Big b1; double pre2 = ms([&] { for (int i = 0; i < N / 10; ++i) ++b1; }); sink = b1.secs;
    Big b2; double post2 = ms([&] { for (int i = 0; i < N / 10; ++i) b2++; }); sink = b2.secs;
    std::cout << "Big   : prefix " << pre2 << " ms, postfix " << post2 << " ms (1M ops, heavy object)\n";
    std::vector<int> v(1000, 1); long long s = 0;
    double unchecked = ms([&] { for (int i = 0; i < N; ++i) s += v[i % 1000]; });
    double checked = ms([&] { for (int i = 0; i < N; ++i) { if (i % 1000 >= (int)v.size()) throw std::out_of_range("x"); s += v[i % 1000]; } });
    double at = ms([&] { for (int i = 0; i < N; ++i) s += v.at(i % 1000); });
    sink = (int)s;
    std::cout << "access x10M: unchecked " << unchecked << " ms, manual check " << checked << " ms, at() " << at << " ms\n";
}
