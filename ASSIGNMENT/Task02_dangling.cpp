// Q2: returning a reference to a LOCAL variable -> dangling reference (undefined behaviour)
#include <iostream>
int& bad()
{
    int local = 42;
    return local;            // g++ -Wall: warning: reference to local variable 'local' returned
}
int main()
{
    int& r = bad();          // r refers to a destroyed object
    std::cout << "value through dangling reference: " << r << '\n';   // undefined behaviour
}
