#pragma once
#include "mallocator.hpp"
#include "mvector.hpp"
#include <deque>

#define DQ_CHUNK_BYTE_SIZE 16
#define DQ_INITIAL_MAP_SIZE 8

namespace mlib
{
    namespace internal
    {
        [[nodiscard]] inline static size_t _S_dq_chunk_capacity(const size_t _type_size_)
        {
            return _type_size_ < DQ_CHUNK_BYTE_SIZE ? size_t(DQ_CHUNK_BYTE_SIZE / _type_size_) : size_t(1);
        };
    }

    template <typename T, typename Alloc = mlib::allocator<T>>
    class deque
    {

        using allocator_traits = mlib::allocator_traits<Alloc>;

        template <typename U, typename ChunkMap, typename Index>
        class _DequeIterator
        {
            using value_type = U;
            using pointer = U *;
            using const_pointer = const U *;
            using reference = U &;
            using const_reference = const U &;

            pointer m_Iterator;
            ChunkMap chunk_map;
            Index m_start_index;
            Index m_cursor;
            Index m_end_index;
            size_t chunk_capacity = internal::_S_dq_chunk_capacity(sizeof(value_type));

        public:
            _DequeIterator() : m_Iterator(nullptr), chunk_map(nullptr), m_cursor(0), m_start_index(0), m_end_index(0) {};
            _DequeIterator(pointer _start_loc_, ChunkMap _cmap_, Index _si_, Index _ei_) : m_Iterator(_start_loc_), chunk_map(_cmap_), m_cursor(_si_), m_start_index(_si_), m_end_index(_ei_) {

                                                                                           };

            _DequeIterator &
            operator++()
            {
                Index current_chunk = (m_start_index + (m_cursor)) / chunk_capacity;
                Index current_offset = (m_start_index + (m_cursor)) % chunk_capacity;

                if (m_cursor > chunk_capacity)
                {
                    m_cursor = 0;
                }

                m_cursor++;
                m_Iterator = &chunk_map[current_chunk][current_offset];

                return *this;
            };

            _DequeIterator &operator--()
            {
            }

            bool operator==(const _DequeIterator &_other_)
            {
                return this->m_Iterator == _other_.m_Iterator;
            };

            bool operator!=(const _DequeIterator &_other_)
            {
                return this->m_Iterator != _other_.m_Iterator;
            };

            _DequeIterator operator++(int)
            {
                _DequeIterator temp = *this;
                ++(*this);
                return temp;
            };
            _DequeIterator operator--(int)
            {
                _DequeIterator temp = *this;
                --(*this);
                return temp;
            };

            reference operator*()
            {
                return *m_Iterator;
            };

            ~_DequeIterator() {};
        };

    public:
        using allocator_type = Alloc;
        using value_type = T;
        using size_type = typename allocator_traits::size_type;
        using difference_type = typename allocator_traits::difference_type;
        using pointer = typename allocator_traits::pointer;
        using const_pointer = typename allocator_traits::const_pointer;
        using reference = typename allocator_traits::reference;
        using const_reference = typename allocator_traits::const_reference;
        using iterator = _DequeIterator<T, mlib::vec<pointer>, size_type>;

    private:
        size_type m_chunk_capacity = internal::_S_dq_chunk_capacity(sizeof(value_type));
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
            allocator_traits::construct(&(chunk_map[chunk_map.size() - 1][m_finish++]), _v_);

            m_size++;
        };

        [[nodiscard]] iterator begin() noexcept
        {

            return iterator(&chunk_map[0][m_start], chunk_map, m_start, m_finish);
        };

        [[nodiscard]] iterator end() noexcept
        {
            return iterator(&chunk_map[chunk_map.size() - 1][m_finish], chunk_map, m_start, m_finish);
        };

        void pop_back()
        {
            if (m_size == 0)
                return;
            allocator_traits::destroy(&chunk_map[chunk_map.size() - 1][m_finish--]);
            m_size--;
        };

        void pop_front()
        {
            if (m_size == 0)
                return;
            allocator_traits::destroy(&chunk_map[0][m_start++]);
            m_size--;
        };

        [[nodiscard]] bool empty() const
        {
            return m_size == 0;
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