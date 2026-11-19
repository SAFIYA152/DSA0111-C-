// billing_tests.hpp - regression tests with hand-verified expected values
#pragma once
#include "billing.hpp"
#include <cmath>
#include <iomanip>

namespace hosp {

inline bool near(double a, double b) { return std::fabs(a - b) < 1e-6; }

inline int runCoreTests(bool verbose = true)
{
    int fails = 0;
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));            // 1700
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000));       // 47500
    bills.push_back(std::make_unique<DayCareBill>("Kiran", 30000));                  // 30000
    bills.push_back(std::make_unique<EmergencyBill>("Asha", 2000, 3, 500));          // 3500
    const double totals[] = {1700, 47500, 30000, 3500};

    SelfPay self;
    Insurance ins(50000, 20);                                                         // 80% covered
    GovernmentScheme gov({{"OP", 1000}, {"IP", 40000}, {"DC", 25000}, {"ER", 3000}});
    const Payer* payers[] = {&self, &ins, &gov};
    // hand-computed covered amounts [payer][bill]
    const double covered[3][4] = {{0, 0, 0, 0},
                                  {1360, 38000, 24000, 2800},
                                  {1000, 40000, 25000, 3000}};

    if (verbose) std::cout << std::left << std::setw(5) << "Bill" << std::setw(8) << "Total"
                           << std::setw(12) << "Payer" << std::setw(10) << "Covered"
                           << std::setw(13) << "PatientPays" << "Result\n";
    for (int j = 0; j < 4; ++j) {
        if (!near(bills[j]->total(), totals[j])) ++fails;
        for (int i = 0; i < 3; ++i) {
            double c = payers[i]->amountCovered(*bills[j]);
            bool ok = near(c, covered[i][j]) && near(payers[i]->patientPays(*bills[j]), totals[j] - covered[i][j]);
            if (!ok) ++fails;
            if (verbose) std::cout << std::setw(5) << bills[j]->type() << std::setw(8) << bills[j]->total()
                                   << std::setw(12) << payers[i]->name() << std::setw(10) << c
                                   << std::setw(13) << payers[i]->patientPays(*bills[j])
                                   << (ok ? "PASS" : "FAIL") << '\n';
        }
    }
    bool okDay = near(dayTotal(bills), 82700);
    if (!okDay) ++fails;
    if (verbose) std::cout << "Day total = " << dayTotal(bills) << " (expected 82700) " << (okDay ? "PASS" : "FAIL") << '\n';
    return fails;
}

} // namespace hosp
