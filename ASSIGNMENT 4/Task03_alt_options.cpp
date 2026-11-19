// Q3 alternative: ONE function taking one options structure (new options = new field, no new overload)
#include <iostream>
struct ChargeOptions { double kg = 0; bool express = false; bool insured = false; bool fragile = false; };
double charge(const ChargeOptions& o)
{
    return 30 + 40 * o.kg + (o.express ? 50 : 0) + (o.insured ? 20 : 0) + (o.fragile ? 15 : 0);
}
int main()
{
    ChargeOptions a; a.kg = 2.5;                                   // charge(2.5)
    ChargeOptions b; b.kg = 2.5; b.express = true; b.insured = true; b.fragile = true;   // new option costs one field
    std::cout << charge(a) << ' ' << charge(b) << '\n';
}
