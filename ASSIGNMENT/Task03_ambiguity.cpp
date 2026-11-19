// Q3: the developer's original set. This file is MEANT to fail to compile.
#include <iostream>
double charge(double weight)                                  { return 30 + 40 * weight; }
double charge(double weight, bool express)                    { return charge(weight) + (express ? 50 : 0); }
double charge(double weight, bool express = false, bool insured = false)
                                                              { return charge(weight, express) + (insured ? 20 : 0); }
int main()
{
    std::cout << charge(2.5) << '\n';   // ambiguous: all three declarations accept one double
}
