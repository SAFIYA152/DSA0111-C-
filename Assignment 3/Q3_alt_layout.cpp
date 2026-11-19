// Flat block vs vector<vector> vs array of row pointers: allocation counts
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>
static long allocs = 0;
void* operator new(std::size_t n) { ++allocs; void* p = std::malloc(n); if (!p) throw std::bad_alloc(); return p; }
void* operator new[](std::size_t n) { ++allocs; void* p = std::malloc(n); if (!p) throw std::bad_alloc(); return p; }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void operator delete[](void* p, std::size_t) noexcept { std::free(p); }
int main() {
    const int R = 100, S = 200; long a0;
    a0 = allocs; { char** m = new char*[R]; for (int i = 0; i < R; ++i) m[i] = new char[S]; for (int i = 0; i < R; ++i) delete[] m[i]; delete[] m; }
    std::cout << "row pointers (new): " << allocs - a0 << " allocations\n";
    a0 = allocs; { char* m = new char[R * S]; m[5 * S + 7] = 'X'; delete[] m; }
    std::cout << "flat block (r*S+s): " << allocs - a0 << " allocation\n";
    a0 = allocs; { std::vector<std::vector<char>> m(R, std::vector<char>(S, '.')); (void)m; }
    std::cout << "vector<vector>    : " << allocs - a0 << " allocations\n";
    std::cout << "extra pointer space: row-pointers=" << R * sizeof(char*) << " B, vector<vector>=" << R * sizeof(std::vector<char>) << " B, flat=0\n";
}
