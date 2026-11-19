// Q4 - Money with operators
#include <iomanip>
#include <iostream>
class Money {
    long long paise;                                         // exact integer representation
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    Money operator+(const Money& o) const { Money r; r.paise = paise + o.paise; return r; }   // new object
    Money operator-(const Money& o) const { Money r; r.paise = paise - o.paise; return r; }   // may be negative (a credit/refund)
    Money& operator+=(const Money& o) { paise += o.paise; return *this; }                      // modifies *this
    bool operator==(const Money& o) const { return paise == o.paise; }
    bool operator<(const Money& o) const { return paise < o.paise; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long v = m.paise < 0 ? -m.paise : m.paise;
        return os << (m.paise < 0 ? "-" : "") << "Rs " << v / 100 << '.' << std::setw(2) << std::setfill('0') << v % 100 << std::setfill(' ');
    }
    friend std::istream& operator>>(std::istream& is, Money& m) { long long r; int p; char dot; is >> r >> dot >> p; if (is) m = Money(r, p); return is; }
};
int main() {
    Money a(0, 75), b(0, 50);
    std::cout << "0.75 + 0.50 = " << a + b << "  (carry)\n";
    std::cout << "0.50 - 0.75 = " << b - a << "  (negative handled with sign)\n";
    // bill calculation with operators
    Money price1(120, 50), price2(79, 25), price3(10), discount(15);
    Money bill = price1 + price2 + price3 - discount;
    bill += Money(0, 99);
    std::cout << "Bill = " << bill << "\n";
    std::cout << "a < b: " << (a < b) << "  b < a: " << (b < a) << "  a == Money(0,75): " << (a == Money(0, 75)) << "\n";
    // double vs Money: 1,000,000 additions of Rs 0.10
    double d = 0; Money m; for (int i = 0; i < 1000000; ++i) { d += 0.10; m += Money(0, 10); }
    std::cout << std::fixed << std::setprecision(6) << "double sum = " << d << " (error " << d - 100000.0 << ")\n";
    std::cout << "Money  sum = " << m << " (exact)\n";
}
