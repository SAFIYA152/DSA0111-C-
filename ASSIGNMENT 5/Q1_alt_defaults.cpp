// Alternative: ONE constructor with default arguments
#include <iostream>
#include <stdexcept>
#include <string>
class Book {
    std::string title, author; int copies;
public:
    Book(const std::string& t = "Untitled", const std::string& a = "Unknown", int c = 0)
        : title(t), author(a), copies(c < 0 ? throw std::invalid_argument("copies") : c) {}   // one check only
    friend std::ostream& operator<<(std::ostream& os, const Book& b) { return os << b.title << "/" << b.author << "/" << b.copies; }
};
int main() {
    Book a, b("Donated"), c("T", "A", 4);
    std::cout << a << "\n" << b << " (donated copies = 0, can't default to 1 w/o ambiguity)\n" << c << "\n";
    Book arr[2]; (void)arr;
}
