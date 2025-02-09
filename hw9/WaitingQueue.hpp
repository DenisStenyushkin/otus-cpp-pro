#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

namespace hw9 {

// Pointer with Mutex
template<typename T>
struct WithMutex {
    template <typename TT>
    WithMutex(TT&& ptr) : m_ptr{std::forward<TT>(ptr)} {}

    T m_ptr;
    std::mutex m_mutex;
};

template<typename T>
struct WaitingQueue {
    WaitingQueue()
        : m_head{new Node}, m_tail{m_head.m_ptr.get()}, m_stopped{false} {}
    
    bool pop(T& value) {
        std::unique_lock lck{m_head.m_mutex};
        m_conditional.wait(lck, [this]() {
            return m_stopped || m_head.m_ptr.get() != get_tail_safe();
        });
        if (m_stopped) {
            return false;
        }

        value = std::move(m_head.m_ptr->m_value);
        take_head_unsafe();
        return true;
    }

    template<typename TT>
    void push(TT& value) {
        std::lock_guard lck{m_tail.m_mutex};

        if (!m_stopped) {
            m_tail.m_ptr->m_value = std::forward<TT>(value);
            m_tail.m_ptr->m_next.reset(new Node{});
            m_tail.m_ptr = m_tail.m_ptr->m_next.get();
            m_conditional.notify_one();
        }
    }

    void stop() {
        std::scoped_lock lck{m_head.m_mutex, m_tail.m_mutex};
        m_stopped = true;
        m_conditional.notify_all();
    }

private:
    struct Node {
        T m_value;
        std::unique_ptr<Node> m_next;
    };

    Node* get_tail_safe() {
        std::lock_guard lck{m_tail.m_mutex};
        return m_tail.m_ptr;
    }

    std::unique_ptr<Node> take_head_unsafe() {
        std::unique_ptr old_head = std::move(m_head.m_ptr);
        m_head.m_ptr = std::move(old_head->m_next);
        return old_head;
    }

    WithMutex<std::unique_ptr<Node>> m_head;
    WithMutex<Node*> m_tail;
    std::condition_variable m_conditional;
    bool m_stopped;
};

} // namespace hw9
