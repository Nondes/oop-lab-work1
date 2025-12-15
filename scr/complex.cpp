#include "complex.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

std::unique_ptr<Number> Complex::clone() const {
    return std::make_unique<Complex>(*this);
}

std::string Complex::str() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6) << re_
       << (im_ < 0 ? " - " : " + ")
       << std::fixed << std::setprecision(6) << std::abs(im_) << "i";
    return ss.str();
}

bool Complex::equals(const Number& other) const {
    if (other.identify() != "complex") return false;
    const auto& b = static_cast<const Complex&>(other);
    const double eps = 1e-9;
    return std::fabs(re_ - b.re_) < eps && std::fabs(im_ - b.im_) < eps;
}

Complex Complex::operator/(const Complex& b) const {
    const double den = b.re_*b.re_ + b.im_*b.im_;
    if (den == 0.0) throw std::runtime_error("complex division by zero");
    return Complex((re_*b.re_ + im_*b.im_) / den, (im_*b.re_ - re_*b.im_) / den);
}

