// Members are initialised in DECLARATION order, not list order
#include <iostream>
class Box {
    int copies;     // declared first
    int total;      // declared second
public:
    Box(int c) : total(copies * 2), copies(c) {}   // wrong: total uses copies BEFORE it is set
    int t() const { return total; }
};
int main() { Box b(5); std::cout << "total (expected 10) = " << b.t() << "\n"; }
