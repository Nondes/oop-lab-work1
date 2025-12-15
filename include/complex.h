#ifndef COMPLEX_H
#define COMPLEX_H

#include "number.h"
#include <memory>
#include <string>

class Complex final : public Number {
public:
    Complex() : re_(0.0), im_(0.0) {}
    Complex(double re, double im) : re_(re), im_(im) {}

    double re() const { return re_; }
    double im() const { return im_; }

    Complex operator+(const Complex& b) const { return Complex(re_ + b.re_, im_ + b.im_); }
    Complex operator-(const Complex& b) const { return Complex(re_ - b.re_, im_ - b.im_); }
    Complex operator*(const Complex& b) const { return Complex(re_*b.re_ - im_*b.im_, re_*b.im_ + im_*b.re_); }
    Complex operator/(const Complex& b) const; // реализация в .cpp

    // Number
    std::unique_ptr<Number> clone() const override;
    std::string identify() const override { return "complex"; }
    std::string str() const override;
    bool equals(const Number& other) const override;
    bool isZero() const override { return re_ == 0.0 && im_ == 0.0; }

private:
    double re_, im_;
};

#endif // COMPLEX_H
