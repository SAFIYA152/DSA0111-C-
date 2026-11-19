// Q3 - dynamic constructor / destructor; -DLEAK skips delete of the heap event
#include <iostream>
#include <stdexcept>
#include <string>
class Event {
    std::string name; int rows, seats; char** map; int booked = 0;
public:
    Event(const std::string& n, int r, int s) : name(n), rows(r), seats(s), map(nullptr) {
        if (r < 0 || s < 0) throw std::invalid_argument("negative size");
        map = new char*[rows];                               // array of row pointers
        for (int i = 0; i < rows; ++i) { map[i] = new char[seats]; for (int j = 0; j < seats; ++j) map[i][j] = '.'; }
        std::cout << "[ctor " << name << " " << rows << "x" << seats << "]\n";
    }
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;
    bool book(int r, int s)   { if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'X') return false; map[r][s] = 'X'; ++booked; return true; }
    bool cancel(int r, int s) { if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] != 'X') return false; map[r][s] = '.'; --booked; return true; }
    ~Event() {
        std::cout << "[dtor " << name << "] summary: " << booked << " of " << rows * seats << " seats booked\n";
        for (int i = 0; i < rows; ++i) delete[] map[i];
        delete[] map;
    }
};
int main() {
    Event stack("StackEvent", 3, 4);
    stack.book(0, 0); stack.book(1, 2); stack.cancel(0, 0);
    std::cout << "double booking same seat: " << stack.book(1, 2) << " (0 = refused)\n";
    {
        Event block("BlockEvent", 2, 2);
        block.book(0, 1);
        std::cout << "leaving block\n";
    }
    Event* heap = new Event("HeapEvent", 5, 5);
    heap->book(4, 4);
#ifndef LEAK
    delete heap;
#endif
    Event zero("ZeroRows", 0, 10);
    std::cout << "zero-row booking: " << zero.book(0, 0) << " (0 = refused)\n";
    std::cout << "end of main\n";
}
