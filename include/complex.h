#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdexcept>
#include <cmath>

class Complex {
public:
    Complex() noexcept : re_(0.0), im_(0.0) {}
    Complex(double re, double im) { set(re, im); }
    Complex(const Complex&) = default;
    Complex& operator=(const Complex&) = default;
    ~Complex() = default;

    // доступ
    double re() const noexcept { return re_; }
    double im() const noexcept { return im_; }

    // модификация с проверкой
    void setRe(double v) {
        if (!std::isfinite(v)) throw std::invalid_argument("bad real");
        re_ = v;
    }
    void setIm(double v) {
        if (!std::isfinite(v)) throw std::invalid_argument("bad imag");
        im_ = v;
    }
    void set(double re, double im) {
        if (!std::isfinite(re) || !std::isfinite(im))
            throw std::invalid_argument("bad parts");
        re_ = re; im_ = im;
    }

    // арифметика (не меняет this)
    Complex add(const Complex& b) const noexcept;
    Complex sub(const Complex& b) const noexcept;
    Complex mul(const Complex& b) const noexcept;
    Complex div(const Complex& b) const; // кидает при делении на 0

    // полезные штуки
    Complex conj() const noexcept { return Complex(re_, -im_); }
    Complex neg()  const noexcept { return Complex(-re_, -im_); }
    double  abs()  const noexcept { return std::hypot(re_, im_); }
    Complex inv()  const; // кидает при 0

    // сравнение с допуском
    bool equals(const Complex& b, double eps = 1e-9) const noexcept;

private:
    double re_;
    double im_;
};

#endif // COMPLEX_H
