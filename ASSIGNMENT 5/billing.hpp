// billing.hpp - Hospital billing: polymorphic bills, abstract payers, exception hierarchy
#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

namespace hosp {

// ---------------- Exception hierarchy (A4) ----------------
class BillingError : public std::runtime_error {
public: using std::runtime_error::runtime_error;
};
class InvalidChargeError : public BillingError {          // negative charges, days < 0 ...
public: using BillingError::BillingError;
};
class ClaimError : public BillingError {                  // claim above cover
public: using BillingError::BillingError;
};
class UnknownPackageError : public BillingError {         // no government rate for this bill type
public: using BillingError::BillingError;
};
class MissingPatientError : public BillingError {         // patient record not found
public: using BillingError::BillingError;
};

inline void requireNonNegative(double v, const char* what)
{
    if (v < 0) throw InvalidChargeError(std::string("negative ") + what);
}

// ---------------- Bill hierarchy (A1) ----------------
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;                              // virtual destructor (A2.3)
    virtual double total() const = 0;                       // pure virtual
    virtual std::string type() const = 0;                   // pure virtual
    virtual void print() const                              // virtual, default behaviour
    {
        std::cout << type() << ' ' << patient << ": " << total() << '\n';
    }
    const std::string& patientName() const noexcept { return patient; }
};

class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(std::string p, double c, double t) : Bill(std::move(p)), consultation(c), tests(t)
    { requireNonNegative(c, "consultation"); requireNonNegative(t, "tests"); }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days; double perDay, procedures, medicines;
public:
    InPatientBill(std::string p, int d, double rate, double proc, double med = 0)
        : Bill(std::move(p)), days(d), perDay(rate), procedures(proc), medicines(med)
    {
        if (d < 0) throw InvalidChargeError("negative days");
        requireNonNegative(rate, "room rate"); requireNonNegative(proc, "procedures");
        requireNonNegative(med, "medicines");
    }
    double total() const override { return days * perDay + procedures + medicines; }
    std::string type() const override { return "IP"; }
};

class DayCareBill : public Bill {
    double packagePrice;
public:
    DayCareBill(std::string p, double pkg) : Bill(std::move(p)), packagePrice(pkg)
    { requireNonNegative(pkg, "package price"); }
    double total() const override { return packagePrice; }
    std::string type() const override { return "DC"; }
};

class EmergencyBill : public Bill {
    double base; double hours; double perHour;
public:
    EmergencyBill(std::string p, double b, double h, double r)
        : Bill(std::move(p)), base(b), hours(h), perHour(r)
    { requireNonNegative(b, "base"); requireNonNegative(h, "hours"); requireNonNegative(r, "hourly rate"); }
    double total() const override { return base + hours * perHour; }
    std::string type() const override { return "ER"; }
};

// ---------------- Payer hierarchy (A3) ----------------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;  // pure virtual
    virtual std::string name() const = 0;
    double patientPays(const Bill& b) const { return b.total() - amountCovered(b); }  // non-virtual template step
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0.0; }
    std::string name() const override { return "SelfPay"; }
};

class Insurance : public Payer {
    double coverLimit, coPayPercent;
public:
    Insurance(double limit, double copayPct) : coverLimit(limit), coPayPercent(copayPct)
    {
        if (limit < 0 || copayPct < 0 || copayPct > 100) throw InvalidChargeError("bad policy terms");
    }
    double amountCovered(const Bill& b) const override
    {
        double t = b.total();
        if (t > coverLimit) throw ClaimError("claim exceeds cover limit");
        return t * (100.0 - coPayPercent) / 100.0;
    }
    std::string name() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    std::map<std::string, double> rate;                     // bill type -> fixed package rate
public:
    explicit GovernmentScheme(std::map<std::string, double> r) : rate(std::move(r)) {}
    double amountCovered(const Bill& b) const override
    {
        auto it = rate.find(b.type());
        if (it == rate.end()) throw UnknownPackageError("no package for type " + b.type());
        return std::min(it->second, b.total());
    }
    std::string name() const override { return "Government"; }
};

// ---------------- Day total (A1.3) ----------------
inline double dayTotal(const std::vector<std::unique_ptr<Bill>>& bills)
{
    double s = 0;
    for (const auto& b : bills) s += b->total();            // late binding
    return s;
}

} // namespace hosp
