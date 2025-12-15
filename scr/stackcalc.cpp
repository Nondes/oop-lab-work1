#include "stackcalc.h"
#include <stdexcept>

void StackCalc::clear() { st_.clear(); last_error_.clear(); }

void StackCalc::pushComplex(double re, double im) {
    st_.push_back(std::make_unique<Complex>(re, im));
}

void StackCalc::pushQuaternion(double a, double b, double c, double d) {
    st_.push_back(std::make_unique<Quaternion>(a, b, c, d));
}

bool StackCalc::need(size_t n) {
    if (st_.size() < n) {
        last_error_ = "Недостаточно операндов";
        return false;
    }
    last_error_.clear();
    return true;
}

bool StackCalc::sameTypesTop2() {
    if (st_.size() < 2) return false;
    return st_[st_.size()-1]->identify() == st_[st_.size()-2]->identify();
}

// снимаем b=top, a=second
bool StackCalc::pop2(std::unique_ptr<Number>& a, std::unique_ptr<Number>& b) {
    if (!need(2)) return false;
    b = std::move(st_.back()); st_.pop_back();
    a = std::move(st_.back()); st_.pop_back();
    return true;
}

Quaternion StackCalc::complexAsQuat(const Complex& z) {
    return Quaternion(z.re(), z.im(), 0.0, 0.0);
}

void StackCalc::add() {
    std::unique_ptr<Number> a, b;
    if (!pop2(a,b)) return;

    const std::string tA = a->identify(), tB = b->identify();
    if (tA=="complex" && tB=="complex") {
        auto& x = static_cast<Complex&>(*a);
        auto& y = static_cast<Complex&>(*b);
        auto r = x + y;
        st_.push_back(std::make_unique<Complex>(r));
    } else {
        // приводим оба к кватернионам
        Quaternion qa = (tA=="quaternion") ? static_cast<Quaternion&>(*a) : complexAsQuat(static_cast<Complex&>(*a));
        Quaternion qb = (tB=="quaternion") ? static_cast<Quaternion&>(*b) : complexAsQuat(static_cast<Complex&>(*b));
        st_.push_back(std::make_unique<Quaternion>(qa + qb));
    }
    last_error_.clear();
}

void StackCalc::sub() {
    std::unique_ptr<Number> a, b;
    if (!pop2(a,b)) return;

    const std::string tA = a->identify(), tB = b->identify();
    if (tA=="complex" && tB=="complex") {
        auto& x = static_cast<Complex&>(*a);
        auto& y = static_cast<Complex&>(*b);
        auto r = x - y;
        st_.push_back(std::make_unique<Complex>(r));
    } else {
        Quaternion qa = (tA=="quaternion") ? static_cast<Quaternion&>(*a) : complexAsQuat(static_cast<Complex&>(*a));
        Quaternion qb = (tB=="quaternion") ? static_cast<Quaternion&>(*b) : complexAsQuat(static_cast<Complex&>(*b));
        st_.push_back(std::make_unique<Quaternion>(qa - qb));
    }
    last_error_.clear();
}

void StackCalc::mul() {
    std::unique_ptr<Number> a, b;
    if (!pop2(a,b)) return;

    const std::string tA = a->identify(), tB = b->identify();
    if (tA=="complex" && tB=="complex") {
        auto& x = static_cast<Complex&>(*a);
        auto& y = static_cast<Complex&>(*b);
        auto r = x * y;
        st_.push_back(std::make_unique<Complex>(r));
    } else {
        Quaternion qa = (tA=="quaternion") ? static_cast<Quaternion&>(*a) : complexAsQuat(static_cast<Complex&>(*a));
        Quaternion qb = (tB=="quaternion") ? static_cast<Quaternion&>(*b) : complexAsQuat(static_cast<Complex&>(*b));
        st_.push_back(std::make_unique<Quaternion>(qa * qb));
    }
    last_error_.clear();
}

void StackCalc::div() {
    std::unique_ptr<Number> a, b;
    if (!pop2(a,b)) return;

    try {
        const std::string tA = a->identify(), tB = b->identify();
        if (tA=="complex" && tB=="complex") {
            auto& x = static_cast<Complex&>(*a);
            auto& y = static_cast<Complex&>(*b);
            auto r = x / y; // кидает исключение при делении на 0
            st_.push_back(std::make_unique<Complex>(r));
        } else {
            Quaternion qa = (tA=="quaternion") ? static_cast<Quaternion&>(*a) : complexAsQuat(static_cast<Complex&>(*a));
            Quaternion qb = (tB=="quaternion") ? static_cast<Quaternion&>(*b) : complexAsQuat(static_cast<Complex&>(*b));
            auto r = qa / qb; // кидает исключение при делении на 0
            st_.push_back(std::make_unique<Quaternion>(r));
        }
        last_error_.clear();
    } catch (const std::exception& e) {
        last_error_ = e.what();
        // возвращаем элементы на стек, чтобы пользователь мог исправить
        st_.push_back(std::move(a));
        st_.push_back(std::move(b));
    }
}
