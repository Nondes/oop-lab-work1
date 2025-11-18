#include "complex.h"
#include <stdexcept>
#include <cmath>

bool Complex::equals(const Complex& o, double eps) const noexcept {
    const double dr = re() - o.re();
    const double di = im() - o.im();
    return std::hypot(dr, di) <= eps;
}

Complex Complex::inv() const {
    const double d = re()*re() + im()*im();
    if (d == 0.0) throw std::invalid_argument("inv of zero complex");
    return Complex(re()/d, -im()/d);
}

Number* Complex::add(const Number& b) const {
    const auto* q = dynamic_cast<const Complex*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    return new Complex(re() + q->re(), im() + q->im());
}
Number* Complex::sub(const Number& b) const {
    const auto* q = dynamic_cast<const Complex*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    return new Complex(re() - q->re(), im() - q->im());
}
Number* Complex::mul(const Number& b) const {
    const auto* q = dynamic_cast<const Complex*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    const double a = re(),  b1 = im(), c = q->re(), d = q->im();
    return new Complex(a*c - b1*d, a*d + b1*c);
}
Number* Complex::div(const Number& b) const {
    const auto* q = dynamic_cast<const Complex*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    const double a = re(),  b1 = im(), c = q->re(), d = q->im();
    const double den = c*c + d*d;
    if (den == 0.0) throw std::invalid_argument("div by zero");
    return new Complex((a*c + b1*d)/den, (b1*c - a*d)/den);
}
