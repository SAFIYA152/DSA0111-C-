// Q5: tickets created inside a function called twice - the static counter outlives the objects
#include <iostream>
#include <string>
class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat; std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), name(n) { if (issued < CAPACITY) seat = ++issued; }
    ~BusTicket() { std::cout << "  (destroyed " << name << ")\n"; }          // count is NOT decremented
    static int count() { return issued; }
    void print() const { std::cout << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n'; }
};
int BusTicket::issued = 0;
void bookTwo(const std::string& a, const std::string& b)
{
    BusTicket x(a), y(b);
    x.print(); y.print();
    std::cout << "Issued so far: " << BusTicket::count() << '\n';
}
int main()
{
    std::cout << "call 1\n"; bookTwo("Asha", "Bala");
    std::cout << "call 2\n"; bookTwo("Chitra", "Dev");
    std::cout << "after both calls, Issued = " << BusTicket::count() << '\n';
}
