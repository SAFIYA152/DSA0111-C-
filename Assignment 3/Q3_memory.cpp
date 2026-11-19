// Memory for three sizes + wrong delete forms
#include <iostream>
int main() {
    long sizes[3][2] = {{10, 20}, {100, 200}, {1000, 2000}};
    for (auto& s : sizes) {
        long r = s[0], c = s[1];
        long data = r * c, ptrs = r * (long)sizeof(char*);
        std::cout << r << " x " << c << " : seats = " << data << " B, row pointers = " << ptrs
                  << " B, total = " << data + ptrs << " B, allocations = " << r + 1 << "\n";
    }
}
