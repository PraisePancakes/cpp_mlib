#pragma once
#include "mallocator.hpp"
#include "mvector.hpp"
#include <deque>

namespace mlib
{
    template <typename T, typename Alloc = mlib::allocator<T>>
    class deque
    {
        using allocator_traits = mlib::allocator_traits<Alloc>;

    public:
        using allocator_type = Alloc;
        using value_type = T;
        using size_type = typename allocator_traits::size_type;
        using difference_type = typename allocator_traits::difference_type;
        using pointer = typename allocator_traits::pointer;
        using const_pointer = typename allocator_traits::const_pointer;
        using reference = T &;
        using const_reference = const T &;
        
    private:
    public:
        /*
            pop_front
            pop_back
            push_front
            push_back
        */

        deque() {};

        ~deque() {};
    };

}