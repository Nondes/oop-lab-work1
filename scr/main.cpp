#include "complex.h"
#include "quaternion.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#ifndef RUN_CLI
#define RUN_TESTS 1
#endif

#ifdef RUN_TESTS
static void testComplex() {
    Complex a(3,4), b(1,-2);
    std::unique_ptr<Number> s(a.add(b));
    auto* sc = dynamic_cast<Complex*>(s.get());
    assert(sc && sc->equals(Complex(4,2)));
    std::unique_ptr<Number> m(a.mul(b));
    auto* mc = dynamic_cast<Complex*>(m.get());
    assert(mc && mc->equals(Complex(11,-2)));
    Complex q = a.inv();
    assert(q.equals(Complex(0.12, -0.16), 1e-12));
}
static void testQuaternion() {
    Quaternion x(1,2,3,4);
    Quaternion y(5,6,7,8);
    std::unique_ptr<Number> s(x.add(y));
    auto* sq = dynamic_cast<Quaternion*>(s.get());
    assert(sq && sq->equals(Quaternion(6,8,10,12)));
    std::unique_ptr<Number> d(x.sub(y));
    auto* dq = dynamic_cast<Quaternion*>(d.get());
    assert(dq && dq->equals(Quaternion(-4,-4,-4,-4)));
    std::unique_ptr<Number> m(x.mul(y));
    auto* mq = dynamic_cast<Quaternion*>(m.get());
    assert(mq && mq->equals(Quaternion(-60,12,30,24)));
    Quaternion invx = x.inv();
    std::unique_ptr<Number> id(x.mul(invx));
    auto* idq = dynamic_cast<Quaternion*>(id.get());
    assert(idq && idq->equals(Quaternion(1,0,0,0), 1e-9));
    std::unique_ptr<Number> qd(x.div(y));
    auto* qdq = dynamic_cast<Quaternion*>(qd.get());
    std::unique_ptr<Number> back(qdq->mul(y));
    auto* backq = dynamic_cast<Quaternion*>(back.get());
    assert(backq && backq->equals(x, 1e-9));
}
static void testIdentify() {
    Complex c(1,2);
    Quaternion q(1,0,0,0);
    assert(c.identify() == "complex");
    assert(q.identify() == "quaternion");
}
static void runAll() {
    testComplex();
    testQuaternion();
    testIdentify();
    std::cout << "OK\n";
}
#endif

#ifdef RUN_CLI
static void runCli() {
    std::unique_ptr<Number> cur;
    std::string cmd;
    std::cout << "c a b | q a b c d | add|sub|mul|div <type> ... | abs | id | print | exit\n";
    while (std::cin >> cmd) {
        try {
            if (cmd == "c") {
                double a,b; std::cin >> a >> b;
                cur.reset(new Complex(a,b));
                std::cout << "ok\n";
            } else if (cmd == "q") {
                double a,b,c,d; std::cin >> a >> b >> c >> d;
                cur.reset(new Quaternion(a,b,c,d));
                std::cout << "ok\n";
            } else if (cmd == "add" || cmd == "sub" || cmd == "mul" || cmd == "div") {
                if (!cur) { std::cout << "empty\n"; continue; }
                std::string t; std::cin >> t;
                if (t == "c") {
                    double a,b; std::cin >> a >> b;
                    Complex rhs(a,b);
                    std::unique_ptr<Number> res(
                        cmd=="add"?cur->add(rhs):
                        cmd=="sub"?cur->sub(rhs):
                        cmd=="mul"?cur->mul(rhs):
                                   cur->div(rhs));
                    cur.swap(res);
                    std::cout << "ok\n";
                } else if (t == "q") {
                    double a,b,c,d; std::cin >> a >> b >> c >> d;
                    Quaternion rhs(a,b,c,d);
                    std::unique_ptr<Number> res(
                        cmd=="add"?cur->add(rhs):
                        cmd=="sub"?cur->sub(rhs):
                        cmd=="mul"?cur->mul(rhs):
                                   cur->div(rhs));
                    cur.swap(res);
                    std::cout << "ok\n";
                } else {
                    std::cout << "type?\n";
                }
            } else if (cmd == "abs") {
                if (!cur) { std::cout << "empty\n"; continue; }
                std::cout << cur->abs() << "\n";
            } else if (cmd == "id") {
                if (!cur) { std::cout << "empty\n"; continue; }
                std::cout << cur->identify() << "\n";
            } else if (cmd == "print") {
                if (!cur) { std::cout << "empty\n"; continue; }
                if (cur->identify()=="complex") {
                    auto* z = dynamic_cast<Complex*>(cur.get());
                    std::cout << z->re() << " " << z->im() << "\n";
                } else {
                    auto* q = dynamic_cast<Quaternion*>(cur.get());
                    std::cout << q->re() << " " << q->i() << " " << q->j() << " " << q->k() << "\n";
                }
            } else if (cmd == "exit") {
                break;
            } else {
                std::cout << "?\n";
            }
        } catch (const std::exception& e) {
            std::cout << "err: " << e.what() << "\n";
        }
    }
}
#endif

int main() {
#ifdef RUN_CLI
    runCli();
#else
    runAll();
#endif
    return 0;
}
