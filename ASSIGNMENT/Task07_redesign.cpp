// Q7: Original (shift the array) vs Redesign (circular buffer) for the last-N-transactions mini statement.
// Build: g++ -std=c++17 -O2 Q7_redesign.cpp -o q7
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

struct Txn { char type; double amount; };

// ---------- ORIGINAL (Q4): shift left when full, O(N) per add ----------
template <int N> class ShiftHistory {
    Txn last[N]; int count = 0;
public:
    void add(Txn t)
    {
        if (count == N) { for (int i = 1; i < N; ++i) last[i - 1] = last[i]; last[N - 1] = t; }
        else last[count++] = t;
    }
    std::vector<Txn> ordered() const { return std::vector<Txn>(last, last + count); }     // oldest first
};
// ---------- REDESIGN: circular buffer, O(1) per add ----------
template <int N> class RingHistory {
    Txn last[N]; int head = 0, count = 0;                  // head = next slot to write
public:
    void add(Txn t)
    {
        last[head] = t;
        if (++head == N) head = 0;
        if (count < N) ++count;
    }
    std::vector<Txn> ordered() const
    {
        std::vector<Txn> v; v.reserve(count);
        int s = (head - count + N) % N;                    // index of the oldest element
        for (int i = 0; i < count; ++i) v.push_back(last[(s + i) % N]);
        return v;
    }
};

volatile double sink;
template <class H> double timeAdds(long long adds)
{
    H h; auto t0 = std::chrono::steady_clock::now();
    for (long long i = 0; i < adds; ++i) h.add({(i & 1) ? 'D' : 'W', double(i)});
    auto v = h.ordered(); sink = v.empty() ? 0 : v.back().amount;
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
}
template <int N> bool sameResult()
{
    ShiftHistory<N> a; RingHistory<N> b; std::mt19937 g(5);
    for (int i = 0; i < 3 * N + 7; ++i) { Txn t{(char)('D' + g() % 2), double(g() % 1000)}; a.add(t); b.add(t); }
    auto x = a.ordered(), y = b.ordered();
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i) if (x[i].type != y[i].type || x[i].amount != y[i].amount) return false;
    return true;
}
template <int N> void row(long long adds)
{
    double s = timeAdds<ShiftHistory<N>>(adds), r = timeAdds<RingHistory<N>>(adds);
    std::cout << "N=" << N << "\t" << (sameResult<N>() ? "same statement" : "MISMATCH") << "\tshift " << s << " ms\tring " << r
              << " ms\tspeed-up x" << s / r << "\t sizeof shift=" << sizeof(ShiftHistory<N>) << " ring=" << sizeof(RingHistory<N>) << '\n';
}
int main()
{
    const long long adds = 200000;
    std::cout << adds << " transactions added per run (-O2)\n";
    row<10>(adds); row<50>(adds); row<1000>(adds); row<5000>(adds);
}
