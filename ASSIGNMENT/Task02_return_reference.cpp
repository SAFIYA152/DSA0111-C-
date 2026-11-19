// Q2: returning references, const references and copies
#include <iostream>
int& highest(int* a, int n)                { int k = 0; for (int i = 1; i < n; ++i) if (a[i] > a[k]) k = i; return a[k]; }
const int& highestConst(const int* a, int n){ int k = 0; for (int i = 1; i < n; ++i) if (a[i] > a[k]) k = i; return a[k]; }
int highestCopy(const int* a, int n)       { int k = 0; for (int i = 1; i < n; ++i) if (a[i] > a[k]) k = i; return a[k]; }
int indexOfHighest(const int* a, int n)    { int k = 0; for (int i = 1; i < n; ++i) if (a[i] > a[k]) k = i; return k; }
int* pointerToHighest(int* a, int n)       { return &highest(a, n); }

void print(const char* t, const int* a, int n) { std::cout << t; for (int i = 0; i < n; ++i) std::cout << ' ' << a[i]; std::cout << '\n'; }
int main()
{
    int marks[5] = {55, 78, 91, 67, 84};
    print("start                  :", marks, 5);
    highest(marks, 5) += 2;                    // moderation applied directly to the array element
    print("after highest()+=2     :", marks, 5);
    highest(marks, 5) = 100;                   // call on the LEFT of =
    print("after highest()=100    :", marks, 5);
    int c = highestCopy(marks, 5); c += 50;    // copy: array untouched
    print("after copy += 50       :", marks, 5);
    const int& cr = highestConst(marks, 5);    // cr = 100, read-only: "cr += 2;" would not compile
    std::cout << "const ref value        : " << cr << '\n';
    marks[indexOfHighest(marks, 5)] += 2;      // index version
    *pointerToHighest(marks, 5) += 2;          // pointer version
    print("after index and pointer:", marks, 5);
}
