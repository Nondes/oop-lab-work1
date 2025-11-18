#include "quaternion.h"
#include <stdexcept>
#include <cmath>

double Quaternion::abs() const {
    return std::sqrt(re()*re() + i_*i_ + j_*j_ + k_*k_);
}
Quaternion Quaternion::conj() const noexcept {
    return Quaternion(re(), -i_, -j_, -k_);
}
Quaternion Quaternion::inv() const {
    const double n2 = re()*re() + i_*i_ + j_*j_ + k_*k_;
    if (n2 == 0.0) throw std::invalid_argument("inv of zero quaternion");
    Quaternion c = conj();
    return Quaternion(c.re()/n2, c.i()/n2, c.j()/n2, c.k()/n2);
}
bool Quaternion::equals(const Quaternion& o, double eps) const noexcept {
    const double dr = re() - o.re();
    const double di = i_ - o.i_;
    const double dj = j_ - o.j_;
    const double dk = k_ - o.k_;
    return std::sqrt(dr*dr + di*di + dj*dj + dk*dk) <= eps;
}
Number* Quaternion::add(const Number& b) const {
    const auto* q = dynamic_cast<const Quaternion*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    return new Quaternion(re() + q->re(), i_ + q->i_, j_ + q->j_, k_ + q->k_);
}
Number* Quaternion::sub(const Number& b) const {
    const auto* q = dynamic_cast<const Quaternion*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    return new Quaternion(re() - q->re(), i_ - q->i_, j_ - q->j_, k_ - q->k_);
}
Number* Quaternion::mul(const Number& b) const {
    const auto* q = dynamic_cast<const Quaternion*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    const double a1 = re(), b1 = i_, c1 = j_, d1 = k_;
    const double a2 = q->re(), b2 = q->i(), c2 = q->j(), d2 = q->k();
    const double a = a1*a2 - b1*b2 - c1*c2 - d1*d2;
    const double b_ = a1*b2 + b1*a2 + c1*d2 - d1*c2;
    const double c_ = a1*c2 - b1*d2 + c1*a2 + d1*b2;
    const double d_ = a1*d2 + b1*c2 - c1*b2 + d1*a2;
    return new Quaternion(a, b_, c_, d_);
}
Number* Quaternion::div(const Number& b) const {
    const auto* q = dynamic_cast<const Quaternion*>(&b);
    if (!q) throw std::invalid_argument("type mismatch");
    Quaternion invb = q->inv();
    return this->mul(invb);
}
