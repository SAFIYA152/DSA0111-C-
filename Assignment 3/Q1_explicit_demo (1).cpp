// Compile normally: OK.  Compile with -DEXPLICIT: error.
#include <iostream>
class Shelf {
    int capacity;
public:
#ifdef EXPLICIT
    explicit Shelf(int c) : capacity(c) {}
#else
    Shelf(int c) : capacity(c) {}
#endif
    int cap() const { return capacity; }
};
void inspect(Shelf s) { std::cout << "shelf capacity " << s.cap() << "\n"; }
int main() { inspect(40); Shelf s = 7; std::cout << s.cap() << "\n"; }   // silent int -> Shelf conversion
