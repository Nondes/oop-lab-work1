#ifndef NUMBER_H
#define NUMBER_H
#include <stdexcept>
#include <cmath>
#include <string>
class Number {
public:
    virtual ~Number() = default;
    double re() const noexcept { return re_; }
    void setRe(double v){ if(!std::isfinite(v)) throw std::invalid_argument("bad real"); re_=v; }
    virtual std::string identify() const = 0;
    virtual Number* add(const Number& b) const = 0;
    virtual Number* sub(const Number& b) const = 0;
    virtual Number* mul(const Number& b) const = 0;
    virtual Number* div(const Number& b) const = 0;
    virtual double abs() const = 0;
protected:
    Number():re_(0.0) {}
    explicit Number(double r){ setRe(r); }
private:
    double re_;
};
#endif
