// Q1 - Constructors, overloading, delegation, validation
#include <iostream>
#include <stdexcept>
#include <string>
class Book {
    std::string title;      // declaration order = initialisation order
    std::string author;
    int copies;
    static int checked(int c) {                       // the ONLY place copies is validated
        if (c < 0) throw std::invalid_argument("copies must be >= 0");
        return c;
    }
public:
    Book() : Book("Untitled", "Unknown", 0) {}                          // empty record (delegates)
    explicit Book(const std::string& t) : Book(t, "Unknown", 1) {}      // donated book (delegates)
    Book(const std::string& t, const std::string& a, int c)            // full details (target)
        : title(t), author(a), copies(checked(c)) {}
    friend std::ostream& operator<<(std::ostream& os, const Book& b) {
        return os << "Book{" << b.title << ", " << b.author << ", copies=" << b.copies << "}";
    }
};
int main() {
    Book a;                                   // default
    Book b("C++ Primer", "Lippman", 5);       // parameterised
    Book c("Donated Novel");                  // title only
    std::cout << a << "\n" << b << "\n" << c << "\n";
    try { Book bad("Bad", "X", -3); } catch (const std::exception& e) { std::cout << "negative copies rejected: " << e.what() << "\n"; }
    try { Book bad2("Bad2", "X", -1); } catch (const std::exception& e) { std::cout << "negative copies rejected: " << e.what() << "\n"; }
}
