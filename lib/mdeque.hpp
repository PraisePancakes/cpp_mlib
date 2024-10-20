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

        [[nodiscard]] inline static size_type _S_dq_chunk_capacity(const size_type _type_size_)
        {
            return _type_size_ < DQ_CHUNK_BYTE_SIZE ? size_type(DQ_CHUNK_BYTE_SIZE / _type_size_) : size_type(1);
        };

    private:
        struct _chunk
        {
            pointer block;

            _chunk() : block(nullptr) {};
            _chunk(pointer _block_loc_) : block(_block_loc_) {};

            ~_chunk() {};
        };

        inline static size_type m_chunk_capacity = _S_dq_chunk_capacity(sizeof(value_type));
        size_type m_start;
        size_type m_finish;

        void _initialize_chunk_map()
        {
            chunk_map.resize(DQ_INITIAL_MAP_SIZE, nullptr);
            chunk_map.push_back(new _chunk(allocator_traits::allocate(m_chunk_capacity)));
            m_start = m_chunk_capacity / 2;
            m_finish = m_start;
        };

        mlib::vec<_chunk *> chunk_map;

    public:
        /*
            pop_front
            pop_back
            push_front
            push_back
        */

        deque()
        {
            _initialize_chunk_map();
        };

        void push_back(const_reference _v_)
        {
            if (m_finish == m_chunk_capacity)
            {
                chunk_map.push_back(new _chunk(allocator_traits::allocate(m_chunk_capacity)));
                m_finish = 0;
            }

            chunk_map[chunk_map.size() - 1]->block[m_finish++] = _v_;
        };

        reference operator[](const size_type _index_)
        {
            const size_type block_idx = (m_start + _index_) / m_chunk_capacity;
            const size_type offset = (m_start + _index_) % m_chunk_capacity;

            return chunk_map[block_idx]->block[offset];
        };

        ~deque() {};
    };

}