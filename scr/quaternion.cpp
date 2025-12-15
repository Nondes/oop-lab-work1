#include "quaternion.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

std::unique_ptr<Number> Quaternion::clone() const {
    return std::make_unique<Quaternion>(*this);
}

std::string Quaternion::str() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6)
       << a_
       << (b_ < 0 ? " - " : " + ") << std::abs(b_) << "i"
       << (c_ < 0 ? " - " : " + ") << std::abs(c_) << "j"
       << (d_ < 0 ? " - " : " + ") << std::abs(d_) << "k";
    return ss.str();
}

bool Quaternion::equals(const Number& other) const {
    if (other.identify() != "quaternion") return false;
    const auto& q = static_cast<const Quaternion&>(other);
    const double eps = 1e-9;
    return std::fabs(a_-q.a_)<eps && std::fabs(b_-q.b_)<eps && std::fabs(c_-q.c_)<eps && std::fabs(d_-q.d_)<eps;
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    const double na = a_*q.a_ - b_*q.b_ - c_*q.c_ - d_*q.d_;
    const double nb = a_*q.b_ + b_*q.a_ + c_*q.d_ - d_*q.c_;
    const double nc = a_*q.c_ - b_*q.d_ + c_*q.a_ + d_*q.b_;
    const double nd = a_*q.d_ + b_*q.c_ - c_*q.b_ + d_*q.a_;
    return Quaternion(na, nb, nc, nd);
}

Quaternion Quaternion::operator/(const Quaternion& q) const {
    const double n2 = q.norm2();
    if (n2 == 0.0) throw std::runtime_error("quaternion division by zero");
    const Quaternion inv = Quaternion(q.a_/n2, -q.b_/n2, -q.c_/n2, -q.d_/n2);
    return (*this) * inv;
}




