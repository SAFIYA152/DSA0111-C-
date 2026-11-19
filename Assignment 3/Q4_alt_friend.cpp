// Member vs non-member (friend) operator+ : only the non-member form allows 100 + m
#include <iostream>
class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    friend Money operator+(const Money& a, const Money& b) { Money r; r.paise = a.paise + b.paise; return r; }
    long long p() const { return paise; }
};
class MoneyMember {                       // member version for contrast
    long long paise;
public:
    MoneyMember(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    MoneyMember operator+(const MoneyMember& o) const { MoneyMember r; r.paise = paise + o.paise; return r; }
    long long p() const { return paise; }
};
int main() {
    Money m(5, 50);
    std::cout << "friend:  m + 100 = " << (m + 100).p() << " paise,  100 + m = " << (100 + m).p() << " paise\n";
    MoneyMember mm(5, 50);
    std::cout << "member:  mm + 100 = " << (mm + 100).p() << " paise\n";
    // (100 + mm) does not compile: the left operand 100 is an int, so no member function can be called on it
}
