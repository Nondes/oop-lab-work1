#include "complex.h"

Complex Complex::add(const Complex& b) const noexcept {
    return Complex(re_ + b.re_, im_ + b.im_);
}

Complex Complex::sub(const Complex& b) const noexcept {
    return Complex(re_ - b.re_, im_ - b.im_);
}

Complex Complex::mul(const Complex& b) const noexcept {
    // (a+bi)(c+di) = (ac - bd) + (ad + bc)i
    const double a = re_,   c = b.re_;
    const double d1 = im_,  d2 = b.im_;
    return Complex(a * c - d1 * d2, a * d2 + d1 * c);
}

Complex Complex::div(const Complex& b) const {
    // (a+bi)/(c+di)
    const double c2d2 = b.re_ * b.re_ + b.im_ * b.im_;
    if (c2d2 == 0.0) throw std::invalid_argument("div by zero");
    const double a = re_, d1 = im_, c = b.re_, d2 = b.im_;
    return Complex((a * c + d1 * d2) / c2d2, (d1 * c - a * d2) / c2d2);
}

Complex Complex::inv() const {
    const double den = re_ * re_ + im_ * im_;
    if (den == 0.0) throw std::invalid_argument("inv of zero");
    return Complex(re_ / den, -im_ / den);
}

bool Complex::equals(const Complex& b, double eps) const noexcept {
    const double dr = re_ - b.re_;
    const double di = im_ - b.im_;
    return std::hypot(dr, di) <= eps;
}
