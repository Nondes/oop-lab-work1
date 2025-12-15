#ifndef NUMBER_H
#define NUMBER_H

#include <memory>
#include <string>

class Number {
public:
    virtual ~Number() = default;
    virtual std::unique_ptr<Number> clone() const = 0;      // глубокая копия для истории
    virtual std::string identify() const = 0;                // "complex" / "quaternion"
    virtual std::string str() const = 0;                     // строка для UI
    virtual bool equals(const Number& other) const = 0;      // сравнение по значению
    virtual bool isZero() const = 0;                         // ноль? (деление)
};

#endif // NUMBER_H

