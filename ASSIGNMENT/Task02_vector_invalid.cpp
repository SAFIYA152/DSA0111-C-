// Q2 changed condition: a reference into a std::vector is invalidated when the vector reallocates.
// Build normally: prints addresses. Build with -DREAD_STALE -fsanitize=address: ASan reports heap-use-after-free.
#include <iostream>
#include <vector>
int main()
{
    std::vector<int> marks{55, 78, 91};
    int& top = marks[2];
    std::cout << "capacity " << marks.capacity() << ", &marks[2] = " << (void*)&marks[2] << '\n';
    for (int i = 0; i < 100; ++i) marks.push_back(i);              // vector grows -> reallocates
    std::cout << "capacity " << marks.capacity() << ", &marks[2] = " << (void*)&marks[2]
              << "  (top still refers to the OLD address " << (void*)&top << ")\n";
#ifdef READ_STALE
    std::cout << "reading stale reference: " << top << '\n';        // undefined behaviour
#endif
    // Safe: remember the INDEX and re-resolve after growing
    std::size_t k = 2;
    marks[k] += 2;
    std::cout << "safe index access: " << marks[k] << '\n';
}
