#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

template<typename Derived>
class CacheKeyBase {
    public:
        bool operator==(const Derived& other) const {
            return static_cast<const Derived*>(this)->isEqual(other);
        }
        
        bool operator!=(const Derived& other) const {
            return !(*this == other);
        }
        
        template<typename T>
        static std::size_t combineHash(std::size_t seed, const T& value) {
            std::hash<T> hasher;
            return seed ^ (hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }
        
        template<typename... Args>
        static std::size_t calculateHash(const Args&... args) {
            std::size_t seed = 0;
            ((seed = combineHash(seed, args)), ...);
            return seed;
        }
    };

    template<typename Key>
    struct CacheKeyHashBase {
        std::size_t operator()(const Key& key) const {
            return key.getHash();
        }
};

template<typename Key, typename Value, typename KeyHash = std::hash<Key>>
class CacheManager {
    public:
        CacheManager() = default;
        ~CacheManager() = default;

        void put(const Key& key, const Value& value) {
            cache_[key] = value;
        }

        bool get(const Key& key, Value& value) const {
            auto it = cache_.find(key);
            if (it != cache_.end()) {
                value = it->second;
                return true;
            }
            return false;
        }

        bool contains(const Key& key) const {
            return cache_.find(key) != cache_.end();
        }

        Value getOrCreate(const Key& key, std::function<Value()> creator) {
            auto it = cache_.find(key);
            if (it != cache_.end()) {
                return it->second;
            }
            
            Value value = creator();
            cache_[key] = value;
            return value;
        }

        void clear() {
            cache_.clear();
        }

        size_t size() const {
            return cache_.size();
        }

        bool remove(const Key& key) {
            auto it = cache_.find(key);
            if (it != cache_.end()) {
                cache_.erase(it);
                return true;
            }
            return false;
        }

        bool empty() const {
            return cache_.empty();
        }

        std::vector<Key> getKeys() const {
            std::vector<Key> keys;
            keys.reserve(cache_.size());
            for (const auto& pair : cache_) {
                keys.push_back(pair.first);
            }
            return keys;
        }

        template<typename Predicate>
        size_t removeIf(Predicate pred) {
            size_t removed = 0;
            auto it = cache_.begin();
            while (it != cache_.end()) {
                if (pred(it->first, it->second)) {
                    it = cache_.erase(it);
                    ++removed;
                } else {
                    ++it;
                }
            }
            return removed;
        }

    private:
        std::unordered_map<Key, Value, KeyHash> cache_;
};
