// Remove the default constructor -> array of Books fails (expected compile error)
#include <string>
class Book { std::string title; int copies;
public: Book(const std::string& t, int c) : title(t), copies(c) {} };
int main() { Book shelf[3]; }
