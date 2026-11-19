// Copy counting, timing, self-assignment
#include <chrono>
#include <iostream>
#include <string>
class Prescription {
    int n; std::string* med;
public:
    static int copies;
    explicit Prescription(int c) : n(c), med(new std::string[c]) { for (int i = 0; i < c; ++i) med[i] = "Medicine" + std::to_string(i); }
    Prescription(const Prescription& o) : n(o.n), med(new std::string[o.n]) { ++copies; for (int i = 0; i < n; ++i) med[i] = o.med[i]; }
    Prescription& operator=(const Prescription& o) {
        if (this == &o) { std::cout << "  [self-assignment ignored]\n"; return *this; }
        std::string* f = new std::string[o.n]; for (int i = 0; i < o.n; ++i) f[i] = o.med[i];
        delete[] med; med = f; n = o.n; return *this; }
    ~Prescription() { delete[] med; }
    std::string first() const { return n ? med[0] : ""; }
};
int Prescription::copies = 0;
void byValue(Prescription p) { (void)p; }
void byRef(const Prescription& p) { (void)p; }
int main() {
    Prescription p(3);
    int c0 = Prescription::copies; byValue(p); std::cout << "call by value: " << Prescription::copies - c0 << " copy\n";
    c0 = Prescription::copies; byRef(p); std::cout << "call by const reference: " << Prescription::copies - c0 << " copies\n";
    c0 = Prescription::copies; Prescription q = p; std::cout << "initialisation Prescription q = p: " << Prescription::copies - c0 << " copy\n";
    c0 = Prescription::copies; auto make = [] { return Prescription(2); }; Prescription m = make(); std::cout << "return by value (prvalue, elided in C++17): " << Prescription::copies - c0 << " copies\n";
    std::cout << "total copy-ctor calls: " << Prescription::copies << "\n";
    std::cout << "self-assignment:\n"; p = p; std::cout << "  p still holds " << p.first() << "\n";
    Prescription::copies = 0;
    for (int n : {10, 1000, 100000}) {
        Prescription big(n); const int reps = 200000 / (n < 1000 ? 1 : n / 100 + 1) + 1;
        auto t0 = std::chrono::steady_clock::now();
        for (int r = 0; r < reps; ++r) { Prescription cpy(big); (void)cpy; }
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "n=" << n << " : " << std::chrono::duration<double, std::micro>(t1 - t0).count() / reps << " us per deep copy\n";
    }
}
