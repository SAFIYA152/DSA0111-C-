// Q6 correctness check: either side of noon, same time twice, reversed order
#include <iostream>
#include <cstdlib>
class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};
int minutesBetween(const Time& a, const Time& b) { return std::abs((b.h * 60 + b.m) - (a.h * 60 + a.m)); }
bool earlier(const Time& a, const Time& b)        { return a.h < b.h || (a.h == b.h && a.m < b.m); }

int fails = 0;
void check(const char* name, long got, long want)
{
    bool ok = got == want; if (!ok) ++fails;
    std::cout << (ok ? "PASS " : "FAIL ") << name << ": got " << got << ", expected " << want << '\n';
}
int main()
{
    check("9:45 -> 13:10 minutes", minutesBetween(Time(9, 45), Time(13, 10)), 205);      // 3h25 = 205
    check("13:10 -> 9:45 minutes (reversed)", minutesBetween(Time(13, 10), Time(9, 45)), 205);
    check("11:30 -> 12:30 across noon", minutesBetween(Time(11, 30), Time(12, 30)), 60);
    check("11:50 -> 12:10 across noon", minutesBetween(Time(11, 50), Time(12, 10)), 20);
    check("same time twice minutes", minutesBetween(Time(10, 0), Time(10, 0)), 0);
    check("earlier(9:45, 13:10)", earlier(Time(9, 45), Time(13, 10)), 1);
    check("earlier(13:10, 9:45)", earlier(Time(13, 10), Time(9, 45)), 0);
    check("earlier(11:59, 12:00)", earlier(Time(11, 59), Time(12, 0)), 1);
    check("earlier(same, same)", earlier(Time(10, 0), Time(10, 0)), 0);
    std::cout << (fails ? "SOME FAILED\n" : "ALL PASSED\n");
    return fails;
}
