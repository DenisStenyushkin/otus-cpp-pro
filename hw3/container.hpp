#pragma once

#include <memory>

#include "allocator.hpp"

namespace hw3 {

template<typename T, typename Allocator = std::allocator<T>>
class LinkedListContainer
{
private:
    struct Node
    {
        Node(Node* n, T v) : next{n}, value{v} {
        }

        Node* next;
        T value;
    };
    using NodeAllocator = typename Allocator::template rebind<Node>::other;

    Node* head;
    NodeAllocator allocator;

    Node* findLastElement() {
        Node * ptr = head;
        if (ptr == nullptr) return nullptr;

        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }
        
        return ptr;
    }

public:
    struct Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit Iterator() : m_ptr{nullptr} {}
        explicit Iterator(Node* ptr) : m_ptr{ptr} {}

        reference operator*() {
            return m_ptr->value;
        }

        pointer operator->() {
            return &(m_ptr->value);
        }

        Iterator& operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator t = *this;
            m_ptr = m_ptr->next;
            return t;
        }

        friend bool operator==(const Iterator& it1, const Iterator& it2) {
            return it1.m_ptr == it2.m_ptr;
        }

        friend bool operator!=(const Iterator& it1, const Iterator& it2) {
            return it1.m_ptr != it2.m_ptr;
        }

    private:
        Node* m_ptr;
    };
    

    LinkedListContainer() : head{nullptr} {}
    
    ~LinkedListContainer() {
        Node* ptr = head;

        while (ptr != nullptr) {
            Node* t = ptr->next;
            allocator.destroy(ptr);
            allocator.deallocate(ptr, 1);
            ptr = t;
        }
    }

    void push_back(const T& v) {
        Node* last = findLastElement();
        Node* t = allocator.allocate(1);
        allocator.construct(t, nullptr, v);
        if (last != nullptr) last->next = t;
        else head = t;
    }

    size_t size() const {
        size_t cntr = 0;
        Node* ptr = head;

        while (ptr != nullptr) {
            ptr = ptr->next;
            ++cntr;
        }

        return cntr;
    }

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

};
