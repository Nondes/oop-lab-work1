#ifndef QUATERNION_H
#define QUATERNION_H

#include "number.h"
#include <memory>
#include <string>

class Quaternion final : public Number {
public:
    Quaternion() : a_(0), b_(0), c_(0), d_(0) {}
    Quaternion(double a, double b, double c, double d) : a_(a), b_(b), c_(c), d_(d) {}

    double re() const { return a_; }
    double i() const { return b_; }
    double j() const { return c_; }
    double k() const { return d_; }

    Quaternion operator+(const Quaternion& q) const { return Quaternion(a_+q.a_, b_+q.b_, c_+q.c_, d_+q.d_); }
    Quaternion operator-(const Quaternion& q) const { return Quaternion(a_-q.a_, b_-q.b_, c_-q.c_, d_-q.d_); }
    Quaternion operator*(const Quaternion& q) const; // реализация в .cpp
    Quaternion operator/(const Quaternion& q) const; // реализация в .cpp

    Quaternion conj() const { return Quaternion(a_, -b_, -c_, -d_); }
    double norm2() const { return a_*a_ + b_*b_ + c_*c_ + d_*d_; }

    // Number
    std::unique_ptr<Number> clone() const override;
    std::string identify() const override { return "quaternion"; }
    std::string str() const override;
    bool equals(const Number& other) const override;
    bool isZero() const override { return a_==0.0 && b_==0.0 && c_==0.0 && d_==0.0; }

private:
    double a_, b_, c_, d_;
};

#endif // QUATERNION_H
