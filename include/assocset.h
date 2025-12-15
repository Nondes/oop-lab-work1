#ifndef ASSOCSET_H
#define ASSOCSET_H

#include <map>
#include <functional>

template<typename K, typename V>
class AssocSet {
public:
    bool insert(const K& k, const V& v) { return data_.emplace(k, v).second; }
    bool contains(const K& k) const { return data_.find(k) != data_.end(); }
    V* find(const K& k) {
        auto it = data_.find(k);
        return it == data_.end() ? nullptr : &it->second;
    }
    const V* find(const K& k) const {
        auto it = data_.find(k);
        return it == data_.end() ? nullptr : &it->second;
    }
    void for_each(const std::function<void(const K&, V&)>& fn) {
        for (auto& kv : data_) fn(kv.first, kv.second);
    }
private:
    std::map<K, V> data_;
};

#endif // ASSOCSET_H
