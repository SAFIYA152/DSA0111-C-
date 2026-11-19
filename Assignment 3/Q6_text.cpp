// Q6 - Text class
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
class Text {
    char* d; std::size_t n;
public:
    static int allocs, copies;
    Text(const char* s = "") : n(std::strlen(s)) { d = new char[n + 1]; std::memcpy(d, s, n + 1); ++allocs; }
    Text(const Text& o) : n(o.n) { d = new char[n + 1]; std::memcpy(d, o.d, n + 1); ++allocs; ++copies; }
    Text& operator=(const Text& o) {
        if (this == &o) return *this;                              // self-assignment guard
        char* t = new char[o.n + 1]; std::memcpy(t, o.d, o.n + 1); ++allocs;
        delete[] d; d = t; n = o.n; return *this; }
    ~Text() { delete[] d; }
    Text& operator+=(const Text& o) {
        char* t = new char[n + o.n + 1]; std::memcpy(t, d, n); std::memcpy(t + n, o.d, o.n + 1); ++allocs;
        delete[] d; d = t; n += o.n; return *this; }
    std::size_t size() const { return n; }
    friend Text operator+(const Text& a, const Text& b) { Text r(a); r += b; return r; }     // version 1
    friend bool operator==(const Text& a, const Text& b) { return a.n == b.n && std::memcmp(a.d, b.d, a.n) == 0; }
    friend bool operator!=(const Text& a, const Text& b) { return !(a == b); }
    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.d; }
};
int Text::allocs = 0, Text::copies = 0;
Text plus2(Text a, const Text& b) { a += b; return a; }                                      // version 2: by value + +=
void reset() { Text::allocs = Text::copies = 0; }
int main() {
    Text a("Hello, "), b("SMS "), c("Gateway");
    reset(); Text r1 = a + b + c;
    std::cout << "v1 a+b+c = \"" << r1 << "\"  copy-ctor calls=" << Text::copies << " allocations=" << Text::allocs << "\n";
    reset(); Text r2 = plus2(plus2(a, b), c);
    std::cout << "v2 by-value  = \"" << r2 << "\"  copy-ctor calls=" << Text::copies << " allocations=" << Text::allocs << "\n";
    Text e; std::cout << "empty: size=" << e.size() << " e+a=\"" << e + a << "\" e==Text(\"\") " << (e == Text("")) << "\n";
    Text big(std::string(5000, 'x').c_str()); Text big2 = big + big;
    std::cout << "long message sizes: " << big.size() << " + " << big.size() << " = " << big2.size() << "\n";
    a = a; std::cout << "self-assign ok: \"" << a << "\"\n";
    std::cout << "a != b: " << (a != b) << "   a == a: " << (a == a) << "\n";
    // timing: 100,000 concatenations
    const int N = 100000;
    auto t0 = std::chrono::steady_clock::now(); Text acc; for (int i = 0; i < N; ++i) acc += Text("x");
    auto t1 = std::chrono::steady_clock::now(); std::string s; for (int i = 0; i < N; ++i) s += "x";
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "100000 appends: Text " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms (O(n^2) exact-size realloc), std::string "
              << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms (amortised O(1))\n";
}
