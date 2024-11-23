#pragma once

#include <cstddef>
#include <cstring>
#include <memory>


namespace hw3 {

struct deleter
{
    void operator()(void* ptr)
    {
        ::operator delete(ptr);
    }
};

template <class T, std::size_t capacity>
struct cpp_11_allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    std::shared_ptr<void> pool;
    std::size_t used = 0;

    cpp_11_allocator () noexcept
        : pool(::operator new (sizeof(T) * capacity), deleter())
    {
    }

    template <class U> cpp_11_allocator(const cpp_11_allocator <U, capacity>& a) noexcept {
        pool = a.pool;
    }

    cpp_11_allocator select_on_container_copy_construction() const
    {
        return cpp_11_allocator();
    }

    T* allocate (std::size_t n)
    {
        T* ret = static_cast<T*>(pool.get()) + used;
        used += n;
        return ret;
    }

    void deallocate ([[maybe_unused]] T* p, [[maybe_unused]] std::size_t n)
    { }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(T* p)
    {
        p->~T();
    }

    template< class U >
    struct rebind
    {
        typedef cpp_11_allocator<U, capacity> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
};

template <class T, class U, std::size_t capacity>
constexpr bool operator== (const cpp_11_allocator<T, capacity>& a1, const cpp_11_allocator<U, capacity>& a2) noexcept
{
    return a1.pool == a2.pool;
}

template <class T, class U, std::size_t capacity>
constexpr bool operator!= (const cpp_11_allocator<T, capacity>& a1, const cpp_11_allocator<U, capacity>& a2) noexcept
{
    return a1.pool != a2.pool;
}

}