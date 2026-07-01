#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

template <class K, class E>
class LinearProbing {
   public:
    explicit LinearProbing(size_t bucket_count)
        : bucket_count_(bucket_count), table_(bucket_count) {
        if (bucket_count == 0) throw std::invalid_argument("Bucket count must be > 0");
    }

    bool Insert(const K& key, const E& value) {
        size_t home = Hash(key);
        size_t first_deleted = bucket_count_;
        size_t j = home;
        do {
            if (table_[j].state == State::Empty) {
                size_t target = (first_deleted != bucket_count_) ? first_deleted : j;
                table_[target] = Entry{key, value, State::Occupied};
                return true;
            }
            if (table_[j].state == State::Deleted && first_deleted == bucket_count_) {
                first_deleted = j;
            } else if (table_[j].state == State::Occupied && table_[j].key == key) {
                table_[j].value = value;
                return false;
            }
            j = Next(j);
        } while (j != home);

        return false;
    }

    bool Erase(const K& key) {
        size_t home = Hash(key);
        size_t j = home;
        do {
            if (table_[j].state == State::Empty) return false;
            if (table_[j].state == State::Occupied && table_[j].key == key) {
                table_[j].state = State::Deleted;
                return true;
            }
            j = Next(j);
        } while (j != home);
        return false;
    }

    std::optional<E> Get(const K& key) const {
        size_t home = Hash(key);
        size_t j = home;
        do {
            if (table_[j].state == State::Empty) return std::nullopt;
            if (table_[j].state == State::Occupied && table_[j].key == key) {
                return table_[j].value;
            }

            j = Next(j);
        } while (j != home);
        return std::nullopt;
    }

   private:
    enum class State { Empty, Occupied, Deleted };
    struct Entry {
        K key{};
        E value{};
        State state{State::Empty};
    };

    size_t Next(size_t i) const { return (i + 1) % bucket_count_; }
    size_t Hash(const K& key) const { return hasher_(key) % bucket_count_; }

    size_t bucket_count_;
    std::vector<Entry> table_;
    std::hash<K> hasher_;
};

int main() {
    LinearProbing<std::string, int> ht(7);
    ht.Insert("a", 1);
    ht.Insert("h", 2);
    ht.Insert("o", 3);

    std::cout << "Before erase, find o: "
              << (ht.Get("o").has_value() ? "found" : "missing") << "\n";
    ht.Erase("h");
    std::cout << "After erasing h, find o (should still be found): "
              << (ht.Get("o").has_value() ? "found" : "missing") << "\n";
    ht.Insert("h", 42);
    std::cout << "After reinserting h into tombstone, value: "
              << ht.Get("h").value_or(-1) << "\n";
    return 0;
}