#pragma once
#include "mallocator.hpp"
#include <deque>

namespace mlib
{
    template <typename T, typename Alloc = mlib::allocator<T>>
    class Deque
    {
        using allocator_traits = mlib::allocator_traits<Alloc>;
        std::deque<int> dq;

    public:
        using allocator_type = Alloc;
        using value_type = T;
        using size_type = typename allocator_traits::size_type;
        using difference_type = typename allocator_traits::difference_type;
        using pointer = typename allocator_traits::pointer;
        using const_pointer = typename allocator_traits::const_pointer;
        using reference = T &;
        using const_reference = const T &;

        Deque() {};
        ~Deque() {};
    };

}