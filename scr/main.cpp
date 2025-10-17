#include "complex.h"
#include <cassert>
#include <iostream>

#ifdef RUN_TESTS
static void testConstructors() {
    Complex z0;
    assert(z0.re() == 0.0 && z0.im() == 0.0);

    Complex z1(3.0, -4.0);
    assert(z1.re() == 3.0 && z1.im() == -4.0);

    Complex z2(z1);
    assert(z2.equals(z1));
}

static void testSetters() {
    Complex z;
    z.set(1.5, -2.5);
    assert(z.re() == 1.5 && z.im() == -2.5);

    bool threw = false;
    try { z.setRe(1.0 / 0.0); } catch (...) { threw = true; }
    assert(threw);
}

static void testOps() {
    Complex a(3, 4), b(1, -2);

    auto s = a.add(b);  // 4 + 2i
    assert(s.equals(Complex(4, 2)));

    auto d = a.sub(b);  // 2 + 6i
    assert(d.equals(Complex(2, 6)));

    auto m = a.mul(b);  // 11 - 2i
    assert(m.equals(Complex(11, -2)));

    Complex c(2, 1);
    auto q = a.div(c);  // (3+4i)/(2+i) = 2 + 1i
    assert(q.equals(Complex(2, 1)));

    bool threw = false;
    try { (void)a.div(Complex(0, 0)); } catch (...) { threw = true; }
    assert(threw);
}

static void testHelpers() {
    Complex z(3, -4);
    assert(z.conj().equals(Complex(3, 4)));
    assert(z.neg().equals(Complex(-3, 4)));
    assert(z.abs() == 5.0);

    auto inv = z.inv(); // (3+4i)/25
    assert(inv.equals(Complex(0.12, 0.16), 1e-12));
}

static void runAll() {
    testConstructors();
    testSetters();
    testOps();
    testHelpers();
    std::cout << "OK\n";
}
#endif

int main() {
#ifdef RUN_TESTS
    runAll();
#else
    Complex x(3, 4), y(1, -2);
    Complex r = x.add(y);
    std::cout << r.re() << " " << r.im() << "\n"; // 4 2
#endif
    return 0;
}
