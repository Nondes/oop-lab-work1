#ifndef HISTORY_H
#define HISTORY_H

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include "number.h"
#include "complex.h"
#include "quaternion.h"
#include "assocset.h"

struct StackSnapshot {
    std::vector<std::unique_ptr<Number>> items;

    // глубокая копия для удобства
    StackSnapshot() = default;
    StackSnapshot(const StackSnapshot& o) {
        items.reserve(o.items.size());
        for (auto& p : o.items) items.push_back(p->clone());
    }
    StackSnapshot& operator=(const StackSnapshot& o) {
        if (this != &o) {
            items.clear();
            items.reserve(o.items.size());
            for (auto& p : o.items) items.push_back(p->clone());
        }
        return *this;
    }

    bool operator==(const StackSnapshot& o) const {
        if (items.size() != o.items.size()) return false;
        for (size_t i = 0; i < items.size(); ++i) {
            const auto* a = items[i].get();
            const auto* b = o.items[i].get();
            if (a->identify() != b->identify()) return false;
            if (!a->equals(*b)) return false;
        }
        return true;
    }

    void serialize(std::ofstream& f) const {
        uint64_t n = items.size();
        f.write(reinterpret_cast<const char*>(&n), sizeof(n));
        for (auto& p : items) {
            char t = (p->identify() == "complex") ? 'c' : 'q';
            f.write(&t, 1);
            if (t == 'c') {
                auto* z = static_cast<const Complex*>(p.get());
                double re = z->re(), im = z->im();
                f.write(reinterpret_cast<const char*>(&re), sizeof(re));
                f.write(reinterpret_cast<const char*>(&im), sizeof(im));
            } else {
                auto* q = static_cast<const Quaternion*>(p.get());
                double a = q->re(), b = q->i(), c = q->j(), d = q->k();
                f.write(reinterpret_cast<const char*>(&a), sizeof(a));
                f.write(reinterpret_cast<const char*>(&b), sizeof(b));
                f.write(reinterpret_cast<const char*>(&c), sizeof(c));
                f.write(reinterpret_cast<const char*>(&d), sizeof(d));
            }
        }
    }

    bool deserialize(std::ifstream& f) {
        uint64_t n = 0;
        if (!f.read(reinterpret_cast<char*>(&n), sizeof(n))) return false;
        std::vector<std::unique_ptr<Number>> tmp;
        tmp.reserve(static_cast<size_t>(n));
        for (uint64_t i = 0; i < n; ++i) {
            char t = 0;
            if (!f.read(&t, 1)) return false;
            if (t == 'c') {
                double re, im;
                if (!f.read(reinterpret_cast<char*>(&re), sizeof(re))) return false;
                if (!f.read(reinterpret_cast<char*>(&im), sizeof(im))) return false;
                tmp.emplace_back(new Complex(re, im));
            } else if (t == 'q') {
                double a, b, c, d;
                if (!f.read(reinterpret_cast<char*>(&a), sizeof(a))) return false;
                if (!f.read(reinterpret_cast<char*>(&b), sizeof(b))) return false;
                if (!f.read(reinterpret_cast<char*>(&c), sizeof(c))) return false;
                if (!f.read(reinterpret_cast<char*>(&d), sizeof(d))) return false;
                tmp.emplace_back(new Quaternion(a, b, c, d));
            } else return false;
        }
        items.swap(tmp);
        return true;
    }
};

using HistorySet = AssocSet<std::string, StackSnapshot>;
std::string now_key();

#endif // HISTORY_H
