// Q1: value / reference / pointer / returned value, plus copy cost of a large object
#include <iostream>
#include <string>

void byValue(long long fee, int p)  { fee -= fee * p / 100; std::cout << "  inside byValue : " << fee << '\n'; }
void byRef(long long& fee, int p)   { fee -= fee * p / 100; }
void byPtr(long long* fee, int p)   { if (fee) *fee -= *fee * p / 100; }
long long returned(long long fee, int p) { return fee - fee * p / 100; }

struct Student {                                   // a "large" object
    std::string name; long long fee; char pad[200];
    static int copies;
    Student(std::string n, long long f) : name(std::move(n)), fee(f), pad{} {}
    Student(const Student& o) : name(o.name), fee(o.fee), pad{} { ++copies; }
};
int Student::copies = 0;
void takeValue(Student) {}
void takeRef(Student&) {}
void takeConstRef(const Student&) {}

int main()
{
    long long f = 45000;
    std::cout << "start            : " << f << '\n';
    byValue(f, 10);  std::cout << "after byValue    : " << f << "  (unchanged)\n";
    byRef(f, 10);    std::cout << "after byRef      : " << f << '\n';
    f = 45000; byPtr(&f, 10);  std::cout << "after byPtr      : " << f << '\n';

    std::cout << "\nSeveral concessions (merit 10, sports 15, single parent 25):\n";
    long long g = 45000;
    byRef(g, 10); byRef(g, 15); byRef(g, 25);
    std::cout << "in place, 3 calls  : " << g << '\n';
    long long h = returned(returned(returned(45000, 10), 15), 25);
    std::cout << "returned, composed : " << h << '\n';
    long long h2 = returned(returned(returned(45000, 25), 15), 10);
    std::cout << "different order    : " << h2 << "  (integer rounding makes order matter)\n";

    std::cout << "\nCopy cost for a large object, sizeof(Student)=" << sizeof(Student) << " bytes\n";
    Student s("Asha", 45000);
    takeValue(s);      std::cout << "copies after takeValue    : " << Student::copies << '\n';
    takeRef(s);        std::cout << "copies after takeRef      : " << Student::copies << '\n';
    takeConstRef(s);   std::cout << "copies after takeConstRef : " << Student::copies << '\n';
}
