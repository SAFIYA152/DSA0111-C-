// Q3: redesigned, unambiguous charge() set
#include <iostream>
struct Parcel { double kg; bool express; bool insured; };

double charge(int grams);                                          // (1) weight in grams
double charge(double kg, bool express = false, bool insured = false);   // (2) weight in kg, optional extras
double charge(const Parcel& p);                                    // (3) a Parcel

double charge(int grams)                         { std::cout << "[grams] ";  return 30 + 40 * (grams / 1000.0); }
double charge(double kg, bool express, bool insured)
                                                 { std::cout << "[kg] ";     return 30 + 40 * kg + (express ? 50 : 0) + (insured ? 20 : 0); }
double charge(const Parcel& p)                   { std::cout << "[parcel] "; return charge(p.kg, p.express, p.insured); }

int main()
{
    Parcel box{2.0, true, false};
    std::cout << "charge(2)           = " << charge(2) << '\n';            // int exact match      -> (1)
    std::cout << "charge(2.0f)        = " << charge(2.0f) << '\n';         // float->double promotion -> (2)
    std::cout << "charge(2.5)         = " << charge(2.5) << '\n';          // double exact          -> (2)
    std::cout << "charge(2.5, true)   = " << charge(2.5, true) << '\n';    //                      -> (2)
    std::cout << "charge(2.5,true,true)= " << charge(2.5, true, true) << '\n';
    std::cout << "charge(box)         = " << charge(box) << '\n';          // Parcel                -> (3)
}
