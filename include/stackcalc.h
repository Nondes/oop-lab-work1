#ifndef STACKCALC_H
#define STACKCALC_H
#include "number.h"
#include "complex.h"
#include "quaternion.h"
#include <memory>
#include <vector>
#include <stdexcept>
enum class Op{Add,Sub,Mul,Div};
class StackCalc{
public:
    void push(std::unique_ptr<Number> n){ st_.push_back(std::move(n)); }
    std::size_t size() const noexcept { return st_.size(); }
    const Number* top() const { if(st_.empty()) throw std::runtime_error("empty"); return st_.back().get(); }
    void pop(){ if(st_.empty()) throw std::runtime_error("empty"); st_.pop_back(); }
    void apply(Op op){
        if(st_.size()<2) throw std::runtime_error("not enough");
        auto b=std::move(st_.back()); st_.pop_back();
        auto a=std::move(st_.back()); st_.pop_back();
        if(a->identify()!=b->identify()) throw std::runtime_error("type mismatch");
        std::unique_ptr<Number> r;
        if(op==Op::Add) r.reset(a->add(*b));
        else if(op==Op::Sub) r.reset(a->sub(*b));
        else if(op==Op::Mul) r.reset(a->mul(*b));
        else r.reset(a->div(*b));
        st_.push_back(std::move(r));
    }
private:
    std::vector<std::unique_ptr<Number>> st_;
};
#endif
