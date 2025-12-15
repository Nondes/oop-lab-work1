#ifndef STACKCALC_H
#define STACKCALC_H

#include <vector>
#include <memory>
#include <string>
#include "number.h"
#include "complex.h"
#include "quaternion.h"

class StackCalc {
public:
    void clear();
    const std::vector<std::unique_ptr<Number>>& data() const { return st_; }
    std::string lastError() const { return last_error_; }

    // ввод
    void pushComplex(double re, double im);
    void pushQuaternion(double a, double b, double c, double d);

    // операции
    void add();
    void sub();
    void mul();
    void div();

private:
    std::vector<std::unique_ptr<Number>> st_;
    std::string last_error_;

    // утилиты
    bool need(size_t n);
    bool sameTypesTop2();
    bool pop2(std::unique_ptr<Number>& a, std::unique_ptr<Number>& b); // a = second from top, b = top
    static Quaternion complexAsQuat(const Complex& z);
};

#endif // STACKCALC_H
