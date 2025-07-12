#include <iostream>
#include <chrono>
#include <unordered_map>
#include <thread>

#include "../include/Option/Option.hpp"

// caching system, where the value may be invalid or missing
template <typename K, typename V>
class Cache {
    struct Entry {
        V value;
        std::chrono::system_clock::time_point expires_at;
    };

    std::unordered_map<K, Entry> data;
    std::chrono::seconds ttl;

public:
    Cache(std::chrono::seconds ttl) : ttl(ttl) {}

    void insert(const K& key, const V& value) {
        data[key] = Entry{value, std::chrono::system_clock::now() + ttl};
    }

    fpp::Option<V> get(const K& key) {
        auto it = data.find(key);
        if (it == data.end()) {
            return fpp::None<V>();
        }

        auto& entry = it->second;
        if (std::chrono::system_clock::now() > entry.expires_at) {
            data.erase(it);
            return fpp::None<V>();
        }

        return fpp::Some(entry.value);
    }
};

int main() {
    Cache<std::string, int> cache(std::chrono::seconds(2));
    cache.insert("price", 100);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto price_first_request = cache.get("price");

    std::cout << "Is price initialized: " << price_first_request.has_value() << "\n";
    if (price_first_request.has_value()){
        std::cout << "Price value: " << price_first_request.value_or_exception() << "\n"; //! all good;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto price_second_request = cache.get("price");
    
    std::cout << "Is price initialized: " << price_second_request.has_value() << "\n";
    if (price_second_request.has_value()){
        std::cout << "Price value: " << price_second_request.value_or_exception() << "\n"; //! expired;
    }

    return 0;
}