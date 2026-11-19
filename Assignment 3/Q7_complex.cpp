// Q7 - Complex numbers for AC impedance
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>
class Complex {
    double re, im;
public:
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}       // default, real-only, full
    double real() const { return re; } double imag() const { return im; }
    double magnitude() const { return std::hypot(re, im); }
    friend Complex operator+(const Complex& a, const Complex& b) { return {a.re + b.re, a.im + b.im}; }
    friend Complex operator-(const Complex& a, const Complex& b) { return {a.re - b.re, a.im - b.im}; }
    friend Complex operator*(const Complex& a, const Complex& b) { return {a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re}; }
    friend Complex operator/(const Complex& a, const Complex& b) {
        double d = b.re * b.re + b.im * b.im;
        if (d == 0.0) throw std::domain_error("division by zero impedance");
        return {(a.re * b.re + a.im * b.im) / d, (a.im * b.re - a.re * b.im) / d};
    }
    friend bool operator==(const Complex& a, const Complex& b) { return std::fabs(a.re - b.re) < 1e-9 && std::fabs(a.im - b.im) < 1e-9; }
    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        return os << std::fixed << std::setprecision(3) << z.re << (z.im < 0 ? " - " : " + ") << std::fabs(z.im) << "j"; }
};
Complex resistor(double R) { return {R, 0}; }
Complex inductor(double w, double L) { return {0, w * L}; }
Complex capacitor(double w, double C) { return {0, -1.0 / (w * C)}; }
Complex parallel(const Complex& z1, const Complex& z2) { return (z1 * z2) / (z1 + z2); }
void show(const char* name, const Complex& z) { std::cout << std::left << std::setw(34) << name << z << "   |Z| = " << z.magnitude() << "\n"; }
int main() {
    const double w = 100.0;                                     // rad/s
    Complex R = resistor(10), L = inductor(w, 0.1);              // j10
    Complex C1 = capacitor(w, 1e-3), C2 = capacitor(w, 5e-4);    // -j10, -j20
    show("R", R); show("L (j10)", L); show("C (-j10)", C1);
    show("Series R+L+C (C=1mF)", R + L + C1);                    // resonance: 10 + 0j
    show("Series R+L+C (C=0.5mF)", R + L + C2);                  // 10 - 10j
    show("Parallel R || L", parallel(R, L));                     // 5 + 5j
    show("Parallel (R||L) || C(0.5mF)", parallel(parallel(R, L), C2));   // 8 + 4j
    std::cout << "hand check R||L == 5+5j : " << (parallel(R, L) == Complex(5, 5)) << "\n";
    std::cout << "hand check (R||L)||C == 8+4j : " << (parallel(parallel(R, L), C2) == Complex(8, 4)) << "\n";
    std::cout << "5 + z works (implicit ctor, friend): " << 5 + Complex(0, 1) << "\n";
    try { std::cout << (R / Complex(0, 0)) << "\n"; } catch (const std::domain_error& e) { std::cout << "caught: " << e.what() << "\n"; }
    // std::complex comparison
    std::complex<double> zs = std::complex<double>(10, 0) * std::complex<double>(0, 10) / (std::complex<double>(10, 0) + std::complex<double>(0, 10));
    std::cout << "std::complex R||L = (" << zs.real() << ", " << zs.imag() << ")\n";
    std::complex<double> inf = std::complex<double>(1, 1) / std::complex<double>(0, 0);
    std::cout << "std::complex 1/0 gives (" << inf.real() << ", " << inf.imag() << ") - silent nan/inf, no exception\n";
}
