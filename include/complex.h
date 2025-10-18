#ifndef COMPLEX_H
#define COMPLEX_H

#include "number.h"

class Complex : public Number {
public:
    Complex() noexcept : Number(0.0), im_(0.0) {}
    Complex(double re, double im) : Number(re) { setIm(im); }

    double im() const noexcept { return im_; }
    void setIm(double v) {
        if (!std::isfinite(v)) throw std::invalid_argument("bad imag");
        im_ = v;
    }

    std::string identify() const override { return "complex"; }

    Number* add(const Number& b) const override;
    Number* sub(const Number& b) const override;
    Number* mul(const Number& b) const override;
    Number* div(const Number& b) const override;

    double abs() const override { return std::hypot(re(), im_); }

    Complex conj() const noexcept { return Complex(re(), -im_); }
    Complex neg()  const noexcept { return Complex(-re(), -im_); }
    Complex inv()  const;

    bool equals(const Complex& o, double eps = 1e-9) const noexcept;

private:
    double im_;
};

#endif
