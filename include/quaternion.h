#ifndef QUATERNION_H
#define QUATERNION_H

#include "number.h"

class Quaternion : public Number {
public:
    Quaternion() noexcept : Number(0.0), i_(0.0), j_(0.0), k_(0.0) {}
    Quaternion(double a, double b, double c, double d) : Number(a) {
        setI(b); setJ(c); setK(d);
    }

    double i() const noexcept { return i_; }
    double j() const noexcept { return j_; }
    double k() const noexcept { return k_; }

    void setI(double v) { check(v); i_ = v; }
    void setJ(double v) { check(v); j_ = v; }
    void setK(double v) { check(v); k_ = v; }

    std::string identify() const override { return "quaternion"; }

    Number* add(const Number& b) const override;
    Number* sub(const Number& b) const override;
    Number* mul(const Number& b) const override;
    Number* div(const Number& b) const override;

    double abs() const override;
    Quaternion conj() const noexcept;
    Quaternion inv()  const;

    bool equals(const Quaternion& o, double eps = 1e-9) const noexcept;

private:
    static void check(double v) {
        if (!std::isfinite(v)) throw std::invalid_argument("bad component");
    }

    double i_, j_, k_;
};

#endif
