#include <cstdint>
#include <functional>
#include <initializer_list>

template <typename Key,
          typename T,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class avl {
 private:
  using key_type = Key;
  using value_type = T;
  // Internal structures
  class Node {
   private:
    using pointer_type = Node*;

    key_type key_;
    value_type value_;
    int16_t height_;
    pointer_type left_child;
    pointer_type right_child;
    pointer_type parent;

   public:
    Node(key_type, value_type);

    int16_t getBalance();
    value_type getValue();
    int16_t updateHeight();

    pointer_type setLeftChild(pointer_type left_child);
    pointer_type setRightChild(pointer_type right_child);
  };
  template <bool IsConst>
  class base_iterator {
   public:
    using pointer_type = std::conditional_t<IsConst, const T*, T*>;
    using reference_type = std::conditional_t<IsConst, const T&, T&>;
    using value_type = T;

    base_iterator(const base_iterator&) = default;
    base_iterator& operator=(const base_iterator&) = default;

    reference_type operator*() const { return *ptr; }
    pointer_type operator->() const { return ptr; }

    base_iterator& operator++() {
      ++ptr;
      return *this;
    }
    base_iterator operator++(int) { base_iterator copy = *this; }
    operator base_iterator<true>() const { return {ptr}; }

   private:
    pointer_type ptr;
    base_iterator(T* ptr) : ptr(ptr) {};
    friend class avl<Key, T>;
  };
  // Bunch of usings
  using type = Node;
  using reference = Node&;
  using const_reference = const Node&;
  using pointer_type = Node*;
  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Root of the tree
  pointer_type root;

  // Private methods for self-balancing
  void balanceTree(pointer_type node);
  void leftRotate(pointer_type node);
  void rightRotate(pointer_type node);
  void setRoot(pointer_type node);

 public:
  avl();
  avl(std::pair<key_type, value_type>);
  avl(std::initializer_list<std::pair<key_type, value_type>>);

  reference at(Key);
  type at(Key) const;

  pointer_type insert(key_type key, value_type value);
  pointer_type insert(std::pair<key_type, value_type> node);

  void erase(key_type key);
  reference find(key_type key);
  type find(key_type key) const;
  bool contains(key_type key) const;
  bool empty() const;
  size_t size() const;

  iterator begin() { return {}; }
  iterator end() { return {}; }
  const_iterator begin() const { return {}; }
  const_iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {}; }
  reverse_iterator rend() const { return {}; }
};
