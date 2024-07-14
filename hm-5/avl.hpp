#pragma once

#include <bits/stl_tree.h>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <ios>
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
  using size_type = size_t;
  Allocator alloc;

  // Internal structures
  struct node {
    value_type value;
    int16_t height = 1;
    node* left_child = nullptr;
    node* right_child = nullptr;
    node* parent = nullptr;

   public:
    node(key_type key, value_type value) : value(std::make_pair(key, value)) {};
    node(value_type value) : value(value) {};

    int16_t getBalance() {
      int16_t result;

      if (!left_child) {
        if (!right_child) {
          result = 0;
        } else {
          result = -right_child->height - 1;
        }
      } else if (!right_child) {
        result = left_child->height + 1;
      } else {
        result = left_child->height - right_child->height;
      }

      return result;
    };

    int16_t updateHeight() {
      if (!left_child) {
        if (!right_child) {
          height = 0;
        } else {
          height = right_child->height + 1;
        }
      } else if (!right_child) {
        height = left_child->height + 1;
      } else {
        height = right_child->height + 1;
      }

      return height;
    };

    node* setLeftChild(node* new_left_child) {
      if (new_left_child != nullptr) {
        new_left_child->parent = this;
      }
      left_child = new_left_child;
      updateHeight();
      return left_child;
    };
    node* setRightChild(node* new_right_child) {
      if (new_right_child != nullptr) {
        new_right_child->parent = this;
      }
      right_child = new_right_child;
      updateHeight();
      return right_child;
    };
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

  using pointer_type = node*;
  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Root of the tree
  pointer_type root = nullptr;

  // Private methods for self-balancing
  void balanceTree(pointer_type node) {
    int16_t node_balance = node->getBalance();
    if (node_balance > 1) {
      if (node->getLeftChild()->getBalance() < 0) {
        leftRotate(node->getLeftChild());
      }
      rightRotate(node);
    } else if (node_balance < -1) {
      if (node->getRightChild()->getBalance() > 0) {
        rightRotate(node->getRightChild());
      }
      leftRotate(node);
    }
  };
  void leftRotate(pointer_type node) {
    pointer_type temp = node->right_child;
    node->right_child = temp->left_child;

    if (temp->left_child) {
      temp->left_child->parent = node;
    }
    if (!node->parent) {
      root = temp;
      temp->parent = nullptr;
    } else if (node->parent->left == node) {
      node->parent->left = temp;
    } else {
      node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
  };
  void rightRotate(pointer_type node) {
    pointer_type temp = node->left_child;
    node->left_child = temp->right_child;

    if (!node->parent) {
      root = temp;
      temp->parent = nullptr;
    } else if (node->parent->left_child == node) {
      node->parent->left_child = temp;
    } else {
      node->parent->right_child = temp;
    }

    temp->right = node;
    node->parent = temp;
  };
  void setRoot(pointer_type node);

 public:
  avl() = default;
  avl(std::initializer_list<value_type> values) {
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
      insert(node(*iter));
    }
  };
  ~avl() {
    for (auto iter = begin(); iter != end(); ++iter) {
      alloc.destroy(*iter);
      alloc.deallocate(*iter);
    }
  };

  T& at(const Key& key);
  const T& at(const Key& key) const;

  std::pair<iterator, bool> insert(const value_type& value);

  iterator erase(const Key& key);

  iterator find(const Key& key);
  const_iterator find(const Key& key) const;

  bool contains(const Key& key) const;

  bool empty() const;
  size_type size() const;

  T& operator[](const Key& key);

  iterator begin() { return {}; }
  iterator end() { return {}; }
  const_iterator begin() const { return {}; }
  const_iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {}; }
  reverse_iterator rend() const { return {}; }
};
