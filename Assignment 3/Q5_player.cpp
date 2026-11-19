// Q5 - Time (++ == <) and Playlist ([] const + non-const)
#include <iostream>
#include <string>
#include <vector>
class Time {
    int secs;   // total seconds, 0..86399
public:
    Time(int h = 0, int m = 0, int s = 0) : secs((h * 3600 + m * 60 + s) % 86400) {}
    Time& operator++() { secs = (secs + 1) % 86400; return *this; }          // prefix: returns reference
    Time operator++(int) { Time old = *this; ++*this; return old; }         // postfix: dummy int, returns COPY
    bool operator==(const Time& o) const { return secs == o.secs; }
    bool operator<(const Time& o) const { return secs < o.secs; }
    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        int h = t.secs / 3600, m = t.secs % 3600 / 60, s = t.secs % 60;
        if (h) os << h << ':';
        os << (h && m < 10 ? "0" : "") << m << ':' << (s < 10 ? "0" : "") << s; return os; }
};
class Playlist {
    std::vector<std::string> tracks; std::string invalid = "<invalid>";
public:
    void add(const std::string& t) { tracks.push_back(t); }
    std::size_t size() const { return tracks.size(); }
    std::string& operator[](int i) {                                  // non-const: allows playlist[i] = "x"
        if (i < 0 || i >= (int)tracks.size()) { std::cout << "[error] index " << i << " out of range (size " << tracks.size() << ")\n"; return invalid; }
        return tracks[i]; }
    const std::string& operator[](int i) const {                      // const: for const Playlist objects
        if (i < 0 || i >= (int)tracks.size()) { std::cout << "[error] index " << i << " out of range (size " << tracks.size() << ")\n"; return invalid; }
        return tracks[i]; }
};
int main() {
    Time t(0, 0, 58);
    Time a = t++;  Time b = ++t;
    std::cout << "a = t++ -> a=" << a << " t=" << t << "\nb = ++t -> b=" << b << " t=" << t << "\n";
    Time r(0, 59, 59); ++r; std::cout << "59:59 + 1s = " << r << "\n";
    Time h(23, 59, 59); ++h; std::cout << "23:59:59 + 1s = " << h << "\n";
    std::cout << "a < b: " << (a < b) << ", a == b: " << (a == b) << "\n";
    Playlist pl; pl.add("Track A"); pl.add("Track B"); pl.add("Track C");
    const Playlist& cpl = pl;
    pl[1] = "Track B (remastered)";
    std::cout << "pl[1] = " << pl[1] << ", const pl[2] = " << cpl[2] << "\n";
    pl[-1]; pl[(int)pl.size()];
    std::cout << "-- playback simulation --\n";
    Time pos(0, 0, 0), end(0, 0, 3);
    for (std::size_t i = 0; i < pl.size(); ++i) { std::cout << "Playing " << pl[i] << ": "; for (Time p = pos; p < end; ++p) std::cout << p << ' '; std::cout << "\n"; }
}
