#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

template <typename Key,
          typename T,
          typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class avl {
 private:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = int16_t;
  Allocator alloc;

  // Internal structures
  struct node {
    const key_type first;
    mapped_type second;
    int16_t height = 1;
    node* left = nullptr;
    node* right = nullptr;
    node* parent = nullptr;

    node(key_type key, mapped_type value) : first(key), second(value) {};
    node(value_type value) : first(value.first), second(value.second) {};

    bool operator<(const node& node) { return first < node.first; }
    bool operator>(const node& node) { return first < first; }
    bool operator==(const node& node) { return first == node.first; }
    friend std::ostream& operator<<(std::ostream& os, const node& node) {
      os << "key: " << node.first << ", value: " << node.second;
      return os;
    }
  };

  template <bool IsConst>
  class base_iterator {
   public:
    using pointer_type = std::conditional_t<IsConst, const node*, node*>;
    using reference_type = std::conditional_t<IsConst, const node&, node&>;
    using value_type = T;

    base_iterator(const base_iterator&) = default;
    base_iterator& operator=(const base_iterator&) = default;

    reference_type operator*() const { return *ptr; }
    pointer_type operator->() const { return ptr; }

    base_iterator& operator++() {
      increment();
      return *this;
    }
    base_iterator operator++(int) {
      base_iterator copy = *this;
      increment();
      return copy;
    }
    base_iterator& operator--() {
      decrement();
      return *this;
    }
    base_iterator operator--(int) {
      base_iterator copy = *this;
      decrement();
      return copy;
    }
    operator base_iterator<true>() const { return {ptr}; }
    operator bool() const { return ptr; }
    bool operator==(const base_iterator& iter) { return ptr == iter.ptr; }
    bool operator!=(const base_iterator& iter) { return ptr != iter.ptr; }

   private:
    node* ptr;
    base_iterator(node* ptr) : ptr(ptr) {};
    friend class avl<Key, T>;

    void decrement() {
      if (ptr->right) {
        ptr = ptr->right;
        while (ptr->left)
          ptr = ptr->left;
      } else {
        node* y = ptr->parent;
        while (ptr == y->right) {
          ptr = y;
          y = y->parent;
        }
        if (ptr->right != y)
          ptr = y;
      }
    }

    void increment() {
      if (ptr->right) {
        ptr = ptr->right;
        while (ptr->left)
          ptr = ptr->left;
      } else {
        node* y = ptr->parent;
        while (ptr == y->right) {
          ptr = y;
          y = y->parent;
        }
        if (ptr->right != y)
          ptr = y;
      }
    }
  };

  using pointer_type = node*;
  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Root of the tree
  pointer_type super_root;
  pointer_type root = nullptr;
  size_type size_ = 0;

  int16_t height(pointer_type node) { return (node) ? node->height : 0; }

  int16_t balancefactor(pointer_type node) {
    return height(node->right) - height(node->left);
  }

  void heightfix(pointer_type node) {
    int16_t left_height = height(node->left);
    int16_t right_height = height(node->right);

    node->height = std::max(left_height, right_height) + 1;
  }

  // Private methods for self-balancing
  pointer_type balance(pointer_type node) {
    heightfix(node);

    if (balancefactor(node) == 2) {
      if (balancefactor(node->right) < 0) {
        node->right = rightrotate(node->right);
      }
      return leftrotate(node);
    }
    if (balancefactor(node) == -2) {
      if (balancefactor(node->left) > 0) {
        node->left = leftrotate(node->left);
      }
      return rightrotate(node);
    }
    return node;
  };
  pointer_type leftrotate(pointer_type node) {
    pointer_type temp = node->right;
    node->right = temp->left;

    if (node->right) {
      node->right->parent = node;
    }

    if (temp->left) {
      temp->left->parent = node;
    }
    if (!node->parent) {
      root = temp;
      super_root->left = root;
      temp->parent = super_root;
    } else if (node->parent->left == node) {
      node->parent->left = temp;
    } else {
      node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;

    heightfix(node);
    heightfix(temp);

    return temp;
  };
  pointer_type rightrotate(pointer_type node) {
    pointer_type temp = node->left;
    node->left = temp->right;

    if (node->left) {
      node->left->parent = node;
    }

    if (!node->parent || node->parent == super_root) {
      root = temp;
      super_root->left = root;
      temp->parent = super_root;
    } else if (node->parent->left == node) {
      node->parent->left = temp;
    } else {
      node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;

    heightfix(node);
    heightfix(temp);

    return temp;
  };

  pointer_type insert_helper(pointer_type current_node,
                             const value_type& value) {
    if (!current_node) {
      return new node(value);
    }
    if (value.first < current_node->first) {
      pointer_type left_child = insert_helper(current_node->left, value);
      current_node->left = left_child;
      left_child->parent = current_node;
    } else if (value.first > current_node->first) {
      pointer_type right_child = insert_helper(current_node->right, value);
      current_node->right = right_child;
      right_child->parent = current_node;
    };

    return balance(current_node);
  }

  void inorder_helper(pointer_type node) {
    if (node != nullptr) {
      inorder_helper(node->left);
      std::cout << "Node : " << node->first << std::endl;
      if (node->parent == nullptr)
        std::cout << "Parent : NULL \n";
      else
        std::cout << "Parent : " << node->parent->first << std::endl;
      inorder_helper(node->right);
    }
  }

  pointer_type successor(pointer_type node) {
    pointer_type temp = node;

    while (temp->left) {
      temp = temp->left;
    }
    return temp;
  }
  pointer_type extractmin(pointer_type node) {
    if (!node->left) {
      return node->right;
    }
    node->left = extractmin(node->left);
    return balance(node);
  }
  pointer_type erase_helper(pointer_type node, const Key& key) {
    if (!node) {
      return nullptr;
    }
    if (key < node->first) {
      pointer_type left_child = erase_helper(node->left, key);
      node->left = left_child;
      left_child->parent = node;
    } else if (key > node->first) {
      pointer_type right_child = erase_helper(node->right, key);
      node->right = right_child;
      right_child->parent = node;
    } else {
      pointer_type l = node->left;
      pointer_type r = node->right;
      pointer_type p = node->parent;
      delete node;

      if (!r) {
        return l;
      }

      pointer_type inorder_successor = successor(r);
      pointer_type temp = extractmin(r);

      inorder_successor->right = temp;
      inorder_successor->left = l;
      inorder_successor->parent = p;

      if (temp) {
        temp->parent = inorder_successor;
      }
      if (l) {
        l->parent = inorder_successor;
      }
      if (!inorder_successor->parent) {
        root = inorder_successor;
      }

      return balance(inorder_successor);
    }

    return balance(node);
  }

 public:
  avl() : super_root(new node(key_type(), mapped_type())) {};
  avl(std::initializer_list<value_type> values)
      : super_root(new node(key_type(), mapped_type())) {
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
      insert(*iter);
    }
  };
  ~avl() {
    // for (auto iter = begin(); iter != end(); ++iter) {
    //   alloc.destroy(*iter);
    //   alloc.deallocate(*iter);
    // }
  };

  void inorder() { inorder_helper(root); }

  T& at(const Key& key) {
    pointer_type node = find(key);
    if (!node) {
      throw std::out_of_range("No element with such key");
    }
    return node->second;
  };
  const T& at(const Key& key) const {
    pointer_type node = find(key);
    if (!node) {
      throw std::out_of_range("No element with such key");
    }
    return node->getConstValue();
  };

  pointer_type insert(const value_type& value) {
    ++size_;
    if (!root) {
      root = new node(value);
      super_root->left = root;
      root->parent = super_root;
      return root;
    }

    return insert_helper(root, value);
  }

  pointer_type erase(const Key& key) {
    if (empty()) {
      throw std::runtime_error("The tree is empty");
    }
    if (!contains(key)) {
      throw std::runtime_error("Element with such key is not in the tree");
    }

    --size_;
    return erase_helper(root, key);
  };

  iterator find(const Key& key) {
    pointer_type temp = root;

    while (temp != nullptr) {
      if (key < temp->first) {
        temp = temp->left;
      } else if (key > temp->first) {
        temp = temp->right;
      } else {
        return iterator(temp);
      }
    }

    return nullptr;
  };
  const_iterator find(const Key& key) const {
    return const_iterator(find(key));
  };

  bool contains(const Key& key) { return find(key) != nullptr; };

  bool empty() const { return !root; };
  size_type size() const { return size_; };
  size_type height() const { return root->height; };

  T& operator[](const Key& key) {
    iterator node = find(key);
    if (!node) {
      insert(value_type(key, T()));
      node = find(key);
    }
    return node->second;
  };

  iterator begin() { return {successor(root)}; }
  iterator end() { return {super_root}; }
  const_iterator begin() const { return {successor(root)}; }
  const_iterator end() const { return {super_root}; }
  reverse_iterator rbegin() const { return {successor(root)}; }
  reverse_iterator rend() const { return {super_root}; }
};
