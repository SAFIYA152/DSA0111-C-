// Q4: SavingsAccount with a private array of the last 10 transactions (shifting design)
#include <iostream>
#include <iomanip>

class SavingsAccount {
    struct Txn { char type; double amount; };
    static const int N = 10;                      // change to 50 to keep the last 50
    long   accNo;
    double balance;
    Txn    last[N];
    int    count;
    void record(char t, double a);                // defined outside
public:
    SavingsAccount(long no, double opening = 0) : accNo(no), balance(opening), count(0) {}   // inside
    double getBalance() const { return balance; }                                             // inside
    void deposit(double amt)                                                                  // inside
    {
        if (amt <= 0) return;
        balance += amt;
        record('D', amt);
    }
    bool withdraw(double amt);                    // outside
    void miniStatement() const;                   // outside
};

void SavingsAccount::record(char t, double a)
{
    if (count == N) {                             // full: shift left, O(N)
        for (int i = 1; i < N; ++i) last[i - 1] = last[i];
        last[N - 1] = {t, a};
    } else {
        last[count++] = {t, a};
    }
}
bool SavingsAccount::withdraw(double amt)
{
    if (amt <= 0 || amt > balance) return false;
    balance -= amt;
    record('W', amt);
    return true;
}
void SavingsAccount::miniStatement() const
{
    std::cout << "Account " << accNo << "  (last " << count << " transactions)\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  " << (last[i].type == 'D' ? "Deposit " : "Withdraw") << std::setw(8) << last[i].amount << '\n';
    std::cout << "  Balance: " << balance << '\n';
}

int main()
{
    SavingsAccount acc(1001, 500);
    for (int i = 1; i <= 12; ++i) {               // 12 transactions: 8 deposits, 4 withdrawals
        if (i % 3 == 0) acc.withdraw(50);
        else            acc.deposit(i * 100);
    }
    acc.miniStatement();                          // only the last 10 are kept
    std::cout << "overdraw 1e9 accepted? " << std::boolalpha << acc.withdraw(1e9) << '\n';
}
