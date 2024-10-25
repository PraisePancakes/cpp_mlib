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

        template <typename U, typename Index>
        class impl_deque_iterator
        {
        public:
            using category = random_access_iterator_tag;
            using this_it = impl_deque_iterator<U, Index>;
            using value_type = U;
            using pointer = U *;
            using reference = U &;
            using const_pointer = const U *;
            using const_reference = const U &;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using deque_type = typename mlib::conditional<mlib::is_const<U>::value, const deque<typename mlib::remove_const<U>::type>, deque<U>>::type;

        private:
            pointer m_iterator;
            deque_type *dq;
            Index m_index;

        public:
            impl_deque_iterator() : m_iterator(nullptr), dq(nullptr), m_index(0) {};
            impl_deque_iterator(deque_type *_dq_, Index _si_) : m_iterator(_dq_->_get_pointer_to_index(_si_)), dq(_dq_),
                                                                m_index(_si_) {};

            this_it &operator++()
            {
                m_iterator = dq->_get_pointer_to_index(++m_index);
                return *this;
            };

            this_it operator++(int)
            {
                this_it temp = *this;
                ++(*this);
                return *this;
            };

            pointer get_iterator() const
            {
                return m_iterator;
            };

            this_it operator--(int)
            {
                this_it temp = *this;
                --(*this);
                return *this;
            }

            this_it &operator--()
            {
                m_iterator = dq->_get_pointer_to_index(--m_index);
                return *this;
            };

            bool operator==(const this_it &_other_of_this_type_)
            {
                return this->m_iterator == _other_of_this_type_.m_iterator;
            };

            bool operator!=(const this_it &_other_of_this_type_)
            {
                return !(*this == _other_of_this_type_);
            }

            reference operator*()
            {
                return *m_iterator;
            };

            ~impl_deque_iterator() {};
        };

    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using const_pointer = const T *;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using map_type = T **;

    public:
        using iterator = impl_deque_iterator<T, size_type>;
        using const_iterator = impl_deque_iterator<const T, size_type>;
        using reverse_iterator = mlib::reverse_iterator<iterator>;
        using const_reverse_iterator = mlib::reverse_iterator<const_iterator>;

    private:
        map_type m_map;             // array of all chunks, each chunk is of capacity = DQ_CHUNK_CAP;
        size_type m_fchunk;         // index of the first chunk allocated
        size_type m_bchunk;         // index of the back chunk allocated
        size_type m_start_index;    // index of the first actual deque index
        size_type m_end_index;      // index of the last actual deque index
        size_type m_map_capacity;   // capacity of map, initally 8
        size_type m_chunk_capacity; // DQ_CHUNK_CAP
        size_type m_size;           // current size of deque (num elems)

        size_type
        _grow_and_copy_at(size_type _off_, size_type _null_range_s_, size_type _null_range_e_)
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

        pointer _get_pointer_to_index(size_t _index_) const
        {
            size_type chk_i = (m_start_index + _index_) / m_chunk_capacity;
            size_type off_i = (m_start_index + _index_) % m_chunk_capacity;

            return &m_map[m_fchunk + chk_i][off_i];
        };

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

        [[nodiscard]] iterator begin()
        {
            return iterator(this, 0);
        }

        [[nodiscard]] const_iterator begin() const
        {
            return const_iterator(this, 0);
        }

        [[nodiscard]] iterator end()
        {
            return iterator(this, this->m_size);
        }

        [[nodiscard]] const_iterator end() const
        {
            return const_iterator(this, this->m_size);
        }

        [[nodiscard]] const_iterator cbegin() const
        {
            return const_iterator(this, 0);
        };

        [[nodiscard]] const_iterator cend() const
        {
            return const_iterator(this, this->m_size);
        };

        [[nodiscard]] reverse_iterator rbegin()
        {
            iterator it(this, this->m_size - 1);
            return reverse_iterator(it);
        };
        [[nodiscard]] reverse_iterator rend()
        {
            iterator it(this, -1);
            return reverse_iterator(it);
        };

        [[nodiscard]] const_reverse_iterator rbegin() const
        {
            const_iterator it(this, this->m_size - 1);
            return const_reverse_iterator(it);
        };
        [[nodiscard]] const_reverse_iterator rend() const
        {
            const_iterator it(this, -1);
            return const_reverse_iterator(it);
        };

        [[nodiscard]] const_reverse_iterator rcbegin() const
        {
            const_iterator it(this, this->m_size - 1);
            return const_reverse_iterator(it);
        };
        [[nodiscard]] const_reverse_iterator rcend() const
        {
            const_iterator it(this, -1);
            return const_reverse_iterator(it);
        };

        inline reference back() const noexcept
        {
            return (*this)[m_size - 1];
        };

        inline reference front() const noexcept
        {
            return (*this)[0];
        }

        inline void pop_back() noexcept
        {
            allocator_traits::destroy(_get_pointer_to_index(m_end_index));
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
            allocator_traits::destroy(_get_pointer_to_index(m_start_index));
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
            pointer i_ptr = _get_pointer_to_index(_index_);
            return *i_ptr;
        };

        [[nodiscard]] inline size_type capacity() const noexcept { return m_map_capacity; };
        [[nodiscard]] inline bool empty() const noexcept { return m_size == 0; };
        [[nodiscard]] inline size_type size() const noexcept { return m_size; };

        ~deque() {};
    };
}
