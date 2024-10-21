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
        inline static size_type m_chunk_capacity = _S_dq_chunk_capacity(sizeof(value_type));
        size_type m_start;
        size_type m_finish;
        size_type m_size = 0;

        void _initialize_chunk_map()
        {
            chunk_map.reserve(DQ_INITIAL_MAP_SIZE);
            chunk_map.push_back(allocator_traits::allocate(m_chunk_capacity));
            m_start = m_chunk_capacity / 2;
            m_finish = m_start;
        };

        mlib::vec<pointer> chunk_map;

    public:
        /*
            pop_front
            pop_back
            push_front
            push_back
        */

        deque() : m_start(0), m_finish(0), m_size(0)
        {
            _initialize_chunk_map();
        };

        void push_back(const_reference _v_)
        {
            if (m_finish == m_chunk_capacity)
            {
                chunk_map.push_back(allocator_traits::allocate(m_chunk_capacity));
                m_finish = 0;
            }
            m_size++;

            allocator_traits::construct(&(chunk_map[chunk_map.size() - 1][m_finish++]), _v_);
        };

        void pop_back()
        {
            allocator_traits::destroy(&chunk_map[chunk_map.size() - 1][m_finish--]);
            m_size--;
        };

        void pop_front()
        {
            allocator_traits::destroy(&chunk_map[0][m_start++]);
            m_size--;
        };

        void push_front(const_reference _v_)
        {
            if (m_start == 0)
            {
                chunk_map.insert(0, allocator_traits::allocate(m_chunk_capacity));
                m_start = m_chunk_capacity;
            }
            m_size++;
            allocator_traits::construct(&(chunk_map[0][--m_start]), _v_);
        };

        [[nodiscard]] inline reference back() const noexcept
        {
            const size_type idx = m_size - 1;
            const size_type block_idx = (m_start + idx) / m_chunk_capacity;
            const size_type offset = (m_start + idx) % m_chunk_capacity;
            return chunk_map[block_idx][offset];
        };

        [[nodiscard]] inline reference front() const noexcept
        {

            const size_type block_idx = (m_start) / m_chunk_capacity;
            const size_type offset = (m_start) % m_chunk_capacity;
            return chunk_map[block_idx][offset];
        };

        [[nodiscard]] inline size_type size() const noexcept
        {
            return m_size;
        };

        reference operator[](const size_type _index_)
        {
            const size_type block_idx = (m_start + _index_) / m_chunk_capacity;
            const size_type offset = (m_start + _index_) % m_chunk_capacity;

            return chunk_map[block_idx][offset];
        };

        ~deque() {};
    };
}