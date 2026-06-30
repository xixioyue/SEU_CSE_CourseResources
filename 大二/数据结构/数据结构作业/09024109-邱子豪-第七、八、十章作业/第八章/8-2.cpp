#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

template <class K, class V, class Hash = std::hash<K>, class KeyCompare = std::less<K>>
class BSTHashTable {
   public:
    BSTHashTable(size_t divisor, double max_load_factor = 0.75, Hash hasher = Hash(),
                 KeyCompare comp = KeyCompare())
        : bucket_count_(divisor),
          max_load_factor_(max_load_factor),
          buckets_(divisor),
          hasher_(std::move(hasher)),
          comp_(std::move(comp)) {
        if (divisor == 0) throw std::invalid_argument("Divisor must be > 0");
        if (divisor % 2 == 0) throw std::invalid_argument("Divisor must be odd");
        if (max_load_factor_ <= 0.0) throw std::invalid_argument("Max load factor must be > 0");
    }

    bool Insert(const K& key, const V& value) {
        bool inserted = InsertNode(buckets_[IndexFor(key)], key, value);
        if (inserted) {
            ++size_;
            if (LoadFactor() > max_load_factor_) {
                Rehash(bucket_count_ * 2 + 1);
            }
        }
        return inserted;
    }

    bool Erase(const K& key) {
        bool removed = EraseNode(buckets_[IndexFor(key)], key);
        if (removed) --size_;
        return removed;
    }

    std::optional<V> Find(const K& key) const {
        const Node* node = FindNode(buckets_[IndexFor(key)], key);
        if (!node) return std::nullopt;
        return node->value;
    }

    size_t Size() const { return size_; }
    size_t BucketCount() const { return bucket_count_; }
    double LoadFactor() const { return bucket_count_ == 0 ? 0.0 : static_cast<double>(size_) / bucket_count_; }

   private:
    struct Node {
        K key;
        V value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(const K& k, const V& v) : key(k), value(v) {}
    };

    size_t IndexFor(const K& key) const { return hasher_(key) % bucket_count_; }

    const Node* FindNode(const std::unique_ptr<Node>& node, const K& key) const {
        if (!node) return nullptr;
        if (comp_(key, node->key)) return FindNode(node->left, key);
        if (comp_(node->key, key)) return FindNode(node->right, key);
        return node.get();
    }

    bool InsertNode(std::unique_ptr<Node>& node, const K& key, const V& value) {
        if (!node) {
            node = std::make_unique<Node>(key, value);
            return true;
        }
        if (comp_(key, node->key)) return InsertNode(node->left, key, value);
        if (comp_(node->key, key)) return InsertNode(node->right, key, value);
        node->value = value;
        return false;
    }

    bool EraseNode(std::unique_ptr<Node>& node, const K& key) {
        if (!node) return false;
        if (comp_(key, node->key)) return EraseNode(node->left, key);
        if (comp_(node->key, key)) return EraseNode(node->right, key);

        if (!node->left) {
            node = std::move(node->right);
        } else if (!node->right) {
            node = std::move(node->left);
        } else {
            Node* successor = MinNode(node->right.get());
            node->key = successor->key;
            node->value = successor->value;
            EraseNode(node->right, successor->key);
        }
        return true;
    }

    Node* MinNode(Node* node) const {
        while (node && node->left) node = node->left.get();
        return node;
    }

    void Rehash(size_t new_bucket_count) {
        if (new_bucket_count % 2 == 0) ++new_bucket_count;
        std::vector<std::unique_ptr<Node>> old_buckets = std::move(buckets_);
        buckets_.clear();
        buckets_.resize(new_bucket_count);
        bucket_count_ = new_bucket_count;
        size_ = 0;
        for (const auto& root : old_buckets) {
            Reinsert(root);
        }
    }

    void Reinsert(const std::unique_ptr<Node>& node) {
        if (!node) return;
        InsertNoResize(node->key, node->value);
        Reinsert(node->left);
        Reinsert(node->right);
    }

    void InsertNoResize(const K& key, const V& value) {
        bool inserted = InsertNode(buckets_[IndexFor(key)], key, value);
        if (inserted) ++size_;
    }

    size_t bucket_count_;
    size_t size_{0};
    double max_load_factor_;
    std::vector<std::unique_ptr<Node>> buckets_;
    Hash hasher_;
    KeyCompare comp_;
};

int main() {
    BSTHashTable<std::string, int> table(7);
    table.Insert("alpha", 1);
    table.Insert("beta", 2);
    table.Insert("gamma", 3);
    table.Insert("delta", 4);
    table.Insert("epsilon", 5);
    table.Insert("zeta", 6);

    std::cout << "Bucket count after inserts: " << table.BucketCount() << "\n";
    std::cout << "Find beta: " << table.Find("beta").value_or(-1) << "\n";
    std::cout << "Erase gamma: " << (table.Erase("gamma") ? "removed" : "missing") << "\n";
    std::cout << "Find gamma after erase: "
              << (table.Find("gamma").has_value() ? "found" : "missing") << "\n";
    std::cout << "Current load factor: " << table.LoadFactor() << "\n";
    return 0;
}


