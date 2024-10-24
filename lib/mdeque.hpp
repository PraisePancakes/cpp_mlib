#pragma once
#include "mallocator.hpp"
#include "mvector.hpp"
#include <deque>

#define DQ_INITIAL_MAP_CAP 8

#define DQ_CHUNK_CAP (sizeof(T) <= 1   ? 16 \
                      : sizeof(T) <= 2 ? 8  \
                      : sizeof(T) <= 4 ? 4  \
                      : sizeof(T) <= 8 ? 2  \
                                       : 1)

namespace mlib
{

    template <typename T, typename Alloc = mlib::allocator<T>>
    class deque
    {
        typedef allocator_traits<Alloc> allocator_traits;

    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using const_pointer = const T *;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using map_type = T **;

    private:
        map_type m_map;             // array of all chunks, each chunk is of capacity = DQ_CHUNK_CAP;
        size_type m_fchunk;         // index of the first chunk allocated
        size_type m_bchunk;         // index of the last chunk allocated
        size_type m_start_index;    // index of the first actual deque index
        size_type m_end_index;      // index of the last actual deque index
        size_type m_map_capacity;   // capacity of map, initally 8
        size_type m_chunk_capacity; // DQ_CHUNK_CAP
        size_type m_size;           // current size of deque (num elems)

        size_type _grow_and_copy_at(size_type _off_, size_type _null_range_s_, size_type _null_range_e_)
        {
            size_type new_map_capacity = m_map_capacity * 2;
            map_type new_map = (T **)malloc(sizeof(T *) * new_map_capacity);
            size_type _old_cap_ret_ = m_map_capacity;

            for (size_type chunk_index = 0; chunk_index < m_map_capacity; ++chunk_index)
            {
                new_map[chunk_index + _off_] = m_map[chunk_index];
                if (chunk_index + _null_range_s_ < _null_range_e_)
                {
                    new_map[chunk_index + _null_range_s_] = nullptr;
                }
            }

            free(m_map);
            m_map = new_map;
            m_map_capacity = new_map_capacity;
            return _old_cap_ret_;
        }

    public:
        deque() : m_map_capacity(DQ_INITIAL_MAP_CAP), m_chunk_capacity(DQ_CHUNK_CAP), m_start_index(0), m_end_index(0), m_fchunk(0), m_bchunk(0), m_size(0)
        {

            m_map = (T **)malloc(sizeof(T *) * DQ_INITIAL_MAP_CAP);
            for (size_t i = 0; i < DQ_INITIAL_MAP_CAP; i++)
            {
                m_map[i] = nullptr;
            }

            m_map[0] = allocator_traits::allocate(DQ_CHUNK_CAP);
        };

        void push_back(const_reference _val_)
        {
            if (m_end_index == DQ_CHUNK_CAP)
            {
                if (m_bchunk == m_map_capacity - 1)
                {
                    // resize map
                    _grow_and_copy_at(0, m_map_capacity, m_map_capacity * 2);
                }

                if (m_map[m_bchunk + 1] == nullptr)
                {
                    m_map[m_bchunk + 1] = allocator_traits::allocate(DQ_CHUNK_CAP);
                }

                m_bchunk++;
                m_end_index = 0;
            }
            allocator_traits::construct(&m_map[m_bchunk][m_end_index], _val_);

            m_end_index++;
            m_size++;
        };
        void push_front(const_reference _val_)
        {
            if (m_start_index == 0)
            {
                if (m_fchunk == 0)
                {
                    // grow the map by DQ_INITIAL_MAP_CAP and copy chunks at DQ_INITIAL_MAP_CAP offset, return old capacity so that we can unwind the front and back chunk indices.
                    size_type old_cap = _grow_and_copy_at(DQ_INITIAL_MAP_CAP, 0, DQ_INITIAL_MAP_CAP);
                    m_fchunk += DQ_INITIAL_MAP_CAP;
                    m_bchunk = m_fchunk + old_cap;
                }
                if (m_map[m_fchunk - 1] == nullptr)
                {
                    m_map[m_fchunk - 1] = allocator_traits::allocate(DQ_CHUNK_CAP);
                }

                m_fchunk--;
                m_start_index = DQ_CHUNK_CAP;
            }

            m_start_index--;
            allocator_traits::construct(&m_map[m_fchunk][m_start_index], _val_);

            m_size++;
        }

        inline reference back() noexcept
        {
            return (*this)[m_size - 1];
        };

        inline reference front() noexcept
        {
            return (*this)[0];
        }

        inline void pop_back() noexcept
        {
            size_type chk_i = (m_start_index + m_end_index) / m_chunk_capacity;
            size_type off_i = (m_start_index + m_end_index) % m_chunk_capacity;
            allocator_traits::destroy(&m_map[m_fchunk + chk_i][off_i]);

            if (m_end_index == 0)
            {
                m_end_index = DQ_CHUNK_CAP;
                m_bchunk--;
            }
            else
            {
                m_end_index--;
            }
            m_size--;
        };
        inline void pop_front() noexcept
        {
            size_type chk_i = (m_start_index) / m_chunk_capacity;
            size_type off_i = (m_start_index) % m_chunk_capacity;

            allocator_traits::destroy(&m_map[m_start_index + chk_i][off_i]); // possibly &back() ?
            if (m_start_index == DQ_CHUNK_CAP - 1)
            {
                m_fchunk++;
                m_start_index = 0;
            }
            else
            {
                m_start_index++;
            }
            m_size--;
        }

        reference operator[](size_type _index_)
        {
            size_type chk_i = (m_start_index + _index_) / m_chunk_capacity;
            size_type off_i = (m_start_index + _index_) % m_chunk_capacity;

            return m_map[m_fchunk + chk_i][off_i];
        };

        [[nodiscard]] inline size_type capacity() const noexcept { return m_map_capacity; };
        [[nodiscard]] inline bool empty() const noexcept { return m_size == 0; };
        [[nodiscard]] inline size_type size() const noexcept { return m_size; };

        ~deque() {};
    };
}
