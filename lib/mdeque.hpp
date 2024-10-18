#pragma once
#include "mallocator.hpp"
#include "mvector.hpp"
#include <deque>

#define DQ_CHUNK_BYTE_SIZE 16
#define DQ_INITIAL_MAP_SIZE 8

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
        using reference = typename allocator_traits::reference;
        using const_reference = typename allocator_traits::const_reference;

    private:
        void _initialize_chunk_map(size_type _type_size_)
        {
            chunk_map.resize(DQ_INITIAL_MAP_SIZE, nullptr);
            chunk_map.push_back(new _chunk(allocate_fixed_size_chunk(_type_size_)));
        };

        [[nodiscard]] inline static size_type _dq_chunk_capacity(const size_type _type_size_)
        {
            return _type_size_ < DQ_CHUNK_BYTE_SIZE ? size_type(DQ_CHUNK_BYTE_SIZE / _type_size_) : size_type(1);
        };

        struct _chunk
        {
            pointer block;
            size_type current_size;

            _chunk() : block(nullptr), current_size(0) {};
            _chunk(pointer _block_loc_) : block(_block_loc_), current_size(0) {};
            ~_chunk() {};

            [[nodiscard]] inline bool is_filled()
            {
                return current_size >= _dq_chunk_capacity(sizeof(value_type));
            };
        };

        mlib::vec<_chunk> chunk_map;

        pointer allocate_fixed_size_chunk(size_type _type_size_)
        {
            return allocator_traits::allocate(_dq_chunk_capacity(_type_size_) / _type_size_);
        };

    public:
        /*
            pop_front
            pop_back
            push_front
            push_back
        */

        deque()
        {
            _initialize_chunk_map(sizeof(value_type));
        };

        void push_back(const_reference _v_) {

        };

        ~deque() {};
    };

}