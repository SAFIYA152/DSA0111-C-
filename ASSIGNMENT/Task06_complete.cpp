#include <iostream>
#include <cstdlib>
class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};
int minutesBetween(const Time& a, const Time& b)
{
    // TODO 1 (done)
    return std::abs((b.h * 60 + b.m) - (a.h * 60 + a.m));
}
bool earlier(const Time& a, const Time& b)
{
    // TODO 2 (done)
    return a.h < b.h || (a.h == b.h && a.m < b.m);
}
int main()
{
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';
}
