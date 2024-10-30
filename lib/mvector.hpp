/****************************************************/
// Filename: mvector.hpp
// Created: Arya Sharifai
// Change history:
// 9.25.2024 / Arya Sharifai
/****************************************************/

#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <exception>
#include <initializer_list>
#include <cstddef>
#include "mallocator.hpp"
#include "miterator.hpp"
#include <limits>

#define _DEF_VECTOR_CAPACITY_ 1
#define QSORT_THRESHOLD 20000

/**
 *  @module
 *    this header implements a dynamic array structure, vector.
 *  This vector supports any derived iterator (see miterator.hpp) as well as any explicitly allocator (see mallocator.hpp).
 *  The purpose of avoiding a "baked-in" allocator/iterator was to enforce the SOC principle, this policy-like design allows for any derived iterator/allocator to
 *  ensure extensibility, separation of algorithms is crucial to this library.
 *
 * @internal module note
 *    To strictly follow an adapter design, make use of allocator_traits and iterator_traits to enforce their respective algorithms / types based on the allocator / iterator respectively.
 *
 */

namespace mlib
{
  /**
   *  @class
   *  vec_base<T, alloc>
   *  this class is a trivial implementation helper that forwards the allocators underlying pointer and is implemented to derive a vector container.
   *  In the case of the mlib::vector 'module' the base soley helps to divide the module into two main concerns of what defines a 'vector' (SoC)
   *    1. the underlying memory region and its respective behaviours.
   *       -- allocating memory for the container
   *       -- initializing and forwarding the 3 'primitive' pointers to the derived.
   *       -- types forwarded from allocator_traits
   *
   *    2. the container (api) itself and its respective behaviours.
   *       -- api functions : reverse(), push_back(), splice(size_t , size_t), etc..
   *       -- iterators
   *       -- types forwarded from allocator_traits
   *  @template
   *  @param
   *  T
   *  represents the underlying container's value_type, this is used to forward to an implicit allocator.
   *  @param
   *  Alloc
   *  this temp initializes a default allocator if no arguments were given.
   *
   */

  template <typename T, class Alloc>
  struct vec_base
  {

  protected:
    typedef Alloc allocator_type;
    typedef typename allocator_traits<Alloc>::pointer pointer;
    allocator_type m_alloc;
    pointer m_region_start;
    pointer m_region_end;
    pointer m_region_capacity;

    vec_base() : m_alloc(nullptr), m_region_start(nullptr), m_region_end(nullptr), m_region_capacity(nullptr) {};

    vec_base(size_t _capacity_)
    {
      m_region_start = m_alloc.allocate(_capacity_);
      m_region_end = m_region_start;
      m_region_capacity = m_region_start + _capacity_;
    }

    vec_base(Alloc _allocator_, size_t _capacity_)
    {
      this->m_alloc = _allocator_;
      m_region_start = m_alloc;
      m_region_end = m_region_start;
      m_region_capacity = m_region_start + _capacity_;
    };

    void _resize_by_offset(const size_t _capacity_size_offset_)
    {

      size_t current_size = m_region_end - m_region_start;
      size_t current_capacity = m_region_capacity - m_region_start;
      size_t new_capacity = current_capacity + _capacity_size_offset_;
      pointer new_start = this->m_alloc.reallocate(this->m_region_start, new_capacity);

      if (!new_start)
      {
        throw std::bad_alloc();
      }

      this->m_region_start = new_start;
      this->m_region_end = new_start + current_size;
      this->m_region_capacity = new_start + new_capacity;
    }

    size_t _calculate_amortized_growth()
    {
      return ((m_region_capacity - m_region_start + 1) << 1);
    };

  public:
    inline size_t size() const
    {
      return m_region_end - m_region_start;
    };

    inline size_t capacity() const
    {
      return m_region_capacity - m_region_start;
    }

    void reserve(size_t _capacity_)
    {
      m_region_start = m_alloc.allocate(_capacity_);
      if (!m_region_start)
      {
        return;
      }
      m_region_end = m_region_start;
      m_region_capacity = m_region_start + _capacity_;
    };

    void resize(size_t _capacity_, const T &_v_)
    {
      reserve(_capacity_);
      for (size_t i = 0; i < _capacity_; i++)
      {
        this->m_alloc.construct(this->m_region_start + i, _v_);
        m_region_end++;
      }
    };

    Alloc &get_allocator() noexcept
    {
      return this->m_alloc;
    };

    void
    clear()
    {
      if (m_region_start)
      {
        for (size_t i = 0; i < size(); i++)
        {
          this->m_alloc.destroy(m_region_start + i);
        }
      }

      m_region_end = m_region_start;
    }

    vec_base(const vec_base &_other_) {

    };

    virtual ~vec_base()
    {
      this->m_alloc.deallocate(m_region_start, 1);
    };
  };

  template <typename T, class Alloc = allocator<T>>
  class vec : public vec_base<T, Alloc>
  {

    typedef allocator_traits<Alloc> allocator_traits;
    typedef allocator_traits::value_type value_type;

    template <typename U>
    class impl_vec_iterator
    {

    public:
      using this_it = impl_vec_iterator<U>;
      using category = random_access_iterator_tag;
      using value_type = U;
      using pointer = U *;
      using const_pointer = const U *;
      using reference = U &;
      using const_reference = const U &;
      using size_type = size_t;
      using difference_type = std::ptrdiff_t;

      pointer m_Iterator;
      impl_vec_iterator() : m_Iterator(nullptr) {};
      impl_vec_iterator(pointer _loc_) : m_Iterator(_loc_) {};

      this_it &operator++()
      {
        ++m_Iterator;
        return *this;
      };

      this_it &operator--()
      {
        --m_Iterator;
        return *this;
      }

      this_it operator++(int)
      {
        this_it temp = *this;
        ++(*this);
        return temp;
      };

      this_it operator--(int)
      {
        this_it temp = *this;
        --(*this);
        return temp;
      };

      this_it operator+(const difference_type _off_)
      {

        return this_it(*this).operator+=(_off_);
      }

      this_it &operator+=(const difference_type _off_)
      {
        m_Iterator = m_Iterator + _off_;
        return *this;
      };

      this_it &operator-=(const difference_type _off_)
      {
        return (*this).operator+=(-_off_);
      };

      bool operator==(const this_it &other)
      {
        return m_Iterator == other.m_Iterator;
      };

      bool operator!=(const this_it &other)
      {
        return m_Iterator != other.m_Iterator;
      }

      reference operator*()
      {
        return *m_Iterator;
      };

      ~impl_vec_iterator() {};
    };

  public:
    typedef typename allocator_traits::pointer pointer;
    typedef typename allocator_traits::const_reference const_reference;
    typedef typename allocator_traits::reference reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    typedef impl_vec_iterator<T> iterator;
    typedef impl_vec_iterator<const T> const_iterator;
    typedef mlib::reverse_iterator<iterator> reverse_iterator;
    typedef mlib::reverse_iterator<const_iterator> const_reverse_iterator;

  private:
    struct ISpliceStrategy
    {
      ISpliceStrategy() {};
      virtual void splice(pointer &_mem_s_, pointer &_mem_e_, size_type _s_index_, size_type _span_, const std::initializer_list<value_type> &_elems_) = 0;
      ~ISpliceStrategy() {};
    };

    struct SpanGreater : public ISpliceStrategy
    {
      SpanGreater() {};
      void splice(pointer &_mem_s_,
                  pointer &_mem_e_,
                  size_type _s_index_,
                  size_type _span_,
                  const std::initializer_list<value_type> &_elems_) override
      {

        size_type i = _s_index_;
        for (size_type index = 0; index < _elems_.size(); index++)
        {
          *(_mem_s_ + i) = _elems_.begin()[index];
          i++;
        }

        size_type _n_lshift = _span_ - _elems_.size();

        i = _s_index_ + _elems_.size();

        for (size_type iter = 0; iter < _n_lshift; iter++)
        {
          for (size_type j = _s_index_ + _elems_.size(); j < _mem_e_ - _mem_s_; j++)
          {
            _mem_s_[j] = _mem_s_[j + 1];
          }

          _mem_e_--;
        }
      };
      ~SpanGreater() {};
    };

    struct SpanLess : public ISpliceStrategy
    {
      SpanLess() {};
      void splice(pointer &_mem_s_,
                  pointer &_mem_e_,
                  size_type _s_index_,
                  size_type _span_,
                  const std::initializer_list<value_type> &_elems_) override
      {

        size_type list_cursor = 0;
        size_type i = _s_index_;
        for (; i < _s_index_ + _span_; ++i)
        {
          _mem_s_[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }
        /*
      void insert(size_type _i_, const_reference _v_)
      {
        size_type si = _i_;
        size_type e = this->m_region_end - this->m_region_start;

        while (e > si)
        {
          *(this->m_region_start + e) = *(this->m_region_start + (e - 1));
          e--;
        }

        *(this->m_region_start + e) = _v_;
        this->m_region_end++;
      };
        */

        for (size_type j = i; j < _s_index_ + _elems_.size(); j++)
        {
          size_type si = j;
          size_type e = _mem_e_ - _mem_s_;

          while (e > si)
          {
            *(_mem_s_ + e) = *(_mem_s_ + (e - 1));
            e--;
          }

          *(_mem_s_ + e) = _elems_.begin()[list_cursor++];
          _mem_e_++;
        }
      };
      ~SpanLess() {};
    };

    struct SpanEquals : public ISpliceStrategy
    {

      SpanEquals() {};
      void splice(pointer &_mem_s_,
                  pointer &_mem_e_,
                  size_type _s_index_,
                  size_type _span_,
                  const std::initializer_list<value_type> &_elems_) override
      {
        size_type list_cursor = 0;
        for (size_type i = _s_index_; i < _s_index_ + _span_; ++i)
        {
          _mem_s_[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }
      };
      ~SpanEquals() {};
    };

    template <typename Strategy, typename = std::enable_if_t<mlib::is_base_of<ISpliceStrategy, Strategy>::value>>
    struct SpliceStrategyAdapter
    {
      Strategy s;
      SpliceStrategyAdapter() {};
      void execute(pointer &_mem_s_,
                   pointer &_mem_e_,
                   size_type _s_index_,
                   size_type _span_,
                   const std::initializer_list<value_type> &_elems_)
      {
        s.splice(_mem_s_, _mem_e_, _s_index_, _span_, _elems_);
      };
      ~SpliceStrategyAdapter() {};
    };

    struct ISortingStrategy
    {
      ISortingStrategy() {};

      virtual void sort(const mlib::vec<value_type> &v) = 0;

      ~ISortingStrategy() {
      };
    };

    struct QuickSort : public ISortingStrategy
    {
    private:
      void _quick_sort(const mlib::vec<value_type> &v, size_type start, size_type end)
      {
        if (end <= start)
          return;

        int pivot = _partition(v, start, end);

        _quick_sort(v, start, pivot - 1);
        _quick_sort(v, pivot + 1, end);
      };

      int _partition(const mlib::vec<value_type> &v, size_type start, size_type end)
      {
        value_type pivot = v[end];

        int i = start - 1;
        for (size_t j = start; j <= end - 1; j++)
        {
          if (v[j] < pivot)
          {
            i++;
            value_type temp = v[i];
            v[i] = v[j];
            v[j] = temp;
          }
        };
        i++;
        value_type temp = v[i];
        v[i] = v[end];
        v[end] = temp;

        return i;
      };

    public:
      QuickSort() {};
      void sort(const mlib::vec<value_type> &v) override
      {
        _quick_sort(v, 0, v.size() - 1);
      };
      ~QuickSort() {};
    };

    template <typename Strategy, typename = std::enable_if_t<mlib::is_base_of<ISortingStrategy, Strategy>::value>>
    struct SortingStrategyAdapter
    {

      SortingStrategyAdapter() {};
      void execute(const mlib::vec<value_type> &v)
      {
        s.sort(v);
      };
      ~SortingStrategyAdapter() {};

    private:
      Strategy s;
    };

  public:
    vec() : vec_base<T, Alloc>(0) {

            };
    vec(size_type _n_) : vec_base<T, Alloc>(_n_) {

                         };

    vec(const vec<value_type> &_other_) : vec_base<T, Alloc>(_other_.capacity())
    {
      for (size_t i = 0; i < _other_.size(); i++)
      {
        push_back(_other_[i]);
      }
    }

    vec(vec<value_type> &&_other_) : vec_base<T, Alloc>(_other_.capacity())
    {
      this->m_region_start = _other_.m_region_start;
      this->m_region_end = _other_.m_region_end;
      this->m_region_capacity = _other_.m_region_capacity;

      _other_.m_region_capacity = nullptr;
      _other_.m_region_start = nullptr;
      _other_.m_region_end = nullptr;
    };

    vec(size_type _n_, const value_type &_v_) : vec_base<T, Alloc>(_n_)
    {

      for (size_type i = 0; i < _n_; i++)
      {

        push_back(_v_);
      }
    }

    vec(const allocator<value_type> &_alloc_, size_type _n_) : vec_base<T, Alloc>(_alloc_, _n_) {

                                                               };

    vec &operator=(const vec &_other_)
    {

      if (this != &_other_)
      {
        this->clear();
        this->_resize_by_offset(_other_.capacity() - this->capacity());
        for (size_t i = 0; i < _other_.size(); i++)
        {
          push_back(_other_[i]);
        }
      }
      return *this;
    }

    /*
        mlib::vec<int> v{2, 3, 4, 5};

        v.insert(2, 5);

    */
    void insert(size_type _i_, const_reference _v_)
    {
      size_type si = _i_;
      size_type e = this->m_region_end - this->m_region_start;

      while (e > si)
      {
        allocator_traits::construct(this->m_region_start + e, *(this->m_region_start + (e - 1)));
        e--;
      }
      allocator_traits::construct(this->m_region_start + e, _v_);
      this->m_region_end++;
    };

    void splice(size_type _s_, size_type _span_, std::initializer_list<value_type> _elems_)
    {

      if (_span_ > _elems_.size())
      {
        SpliceStrategyAdapter<SpanGreater> s;
        s.execute(this->m_region_start, this->m_region_end, _s_, _span_, _elems_);
      }
      else if (_span_ < _elems_.size())
      {
        SpliceStrategyAdapter<SpanLess> s;
        s.execute(this->m_region_start, this->m_region_end, _s_, _span_, _elems_);
      }
      else if (_span_ == _elems_.size())
      {
        SpliceStrategyAdapter<SpanEquals> s;
        s.execute(this->m_region_start, this->m_region_end, _s_, _span_, _elems_);
      }
    };

    vec(std::initializer_list<value_type> _elems_) : vec_base<T, Alloc>(_elems_.size())
    {
      for (auto it = _elems_.begin(); it != _elems_.end(); it++)
      {
        push_back(*it);
      }
    };

    bool operator==(const mlib::vec<value_type> &_other_)
    {

      if (this->size() < _other_.size())
        return false;
      for (size_type i = 0; i < _other_.size(); i++)
      {
        if (*(this->m_region_start + i) != _other_[i])
          return false;
      }

      return true;
    };

    reference operator[](size_type _ptr_index_) const
    {

      return *(this->m_region_start + _ptr_index_);
    };

    reference &at(size_type _ptr_index_) const
    {
      // bound check
      size_type max_value = std::numeric_limits<size_type>::max();

      if (_ptr_index_ >= this->size() || _ptr_index_ == max_value)
      {
        throw std::out_of_range("\nIndex succeeded bound with overflowed/underflowed range index, out of range ");
      }

      return *(this->m_region_start + _ptr_index_);
    };

    iterator begin() const
    {
      return iterator(this->m_region_start);
    };

    iterator end() const
    {
      return iterator(this->m_region_end);
    }

    const_iterator cbegin() const
    {
      return const_iterator(this->m_region_start);
    }

    const_iterator cend() const
    {
      return const_iterator(this->m_region_end);
    }

    reverse_iterator rbegin() const
    {
      return reverse_iterator(this->m_region_end - 1);
    }

    reverse_iterator rend() const
    {
      return reverse_iterator(this->m_region_start - 1);
    }

    bool
    empty() const
    {
      return this->size() == 0;
    }

    void push_back(const_reference _v_)
    {
      if (this->size() >= this->capacity())
      {

        this->_resize_by_offset(this->_calculate_amortized_growth());
      }

      this->m_alloc.construct(this->m_region_start + this->size(), _v_);
      this->m_region_end++;
    }

    /*
      @function
        push_back

      @params
        rvalue reference to _v_

      @brief
        push_back must enforce a growth factor of 2, the constant growth factor ensures the behavior of the respective allocators capacity follows constant amortized time.
        where the operations can be expressed as such :

        insertion => O(1)
        expensive_realloc => if insertion position = 2n : O(2n)
        insertion => O(1)
        expensive_realloc => if insertion position = 2n : O(2n)
        ...
        where n is the size of the vector, with each realloc it will take twice as long to compute but it will also take twice as long to reach the execution of this operation
        which means on average it will take O(n) time to reach the expensive operations, yet the time it takes to reach that operation will always be 2 x previous_time
        meaning with larger sets we have less expensive operations

        in the case that we have an rvalue reference, we simply move that value through perfect forwarded template param in the allocator method allocator::construct which takes an
        rvalue reference to a parameter pack of args.

    */

    void push_back(reference &_v_)
    {
      if (this->size() >= this->capacity())
      {
        this->_resize_by_offset(this->_calculate_amortized_growth());
      }

      this->m_alloc.construct(this->m_region_start + this->size(), _v_);
      this->m_region_end++;
    }

    void push_back(std::initializer_list<value_type> _args_)
    {

      for (auto it = _args_.begin(); it != _args_.end(); ++it)
      {
        push_back(*it);
      }
    }

    void sort()
    {
      if (this->size() <= QSORT_THRESHOLD)
      {
        SortingStrategyAdapter<QuickSort> s;
        s.execute(*this);
      };
    };

    void reverse(size_type _start_, size_type _end_)
    {
      // reverse from start to end.
      if (this->size() == 0)
        return;

      if (this->size() == 1)
        return;

      const size_type temp_start = _start_;
      const size_type temp_end = _end_;

      if (_start_ > _end_)
      {
        _start_ = temp_end;
        _end_ = temp_start;
      }

      size_type j = _end_;
      for (size_type i = _start_; i < j; i++, j--)
      {
        const value_type temp = this->m_region_start[i];
        this->m_region_start[i] = this->m_region_start[j];
        this->m_region_start[j] = temp;
      }
    };

    mlib::vec<value_type> slice(size_type __start__, size_type __end__)
    {
      const int temp_start = __start__;
      const int temp_end = __end__;

      if (__start__ < 0)
      {
        __start__ = 0;
      }

      if (__end__ >= this->size())
      {
        __end__ = this->size();
      }

      if (__start__ > __end__)
      {
        __end__ = temp_start;
        __start__ = temp_end;
      }

      mlib::vec<value_type> v(__end__ - __start__);

      for (size_type i = __start__; i < __end__; i++)
      {
        this->m_alloc.construct(v.m_region_start + i, *(this->m_region_start + i));
        v.m_region_end++;
      }

      return v;
    }

    void pop_back() noexcept
    {
      if (empty())
      {
        return;
      }
      this->m_alloc.destroy(this->m_region_start + this->size());
      this->m_region_end--;
    };

    void print()
    {
      for (size_type i = 0; i < this->size(); i++)
      {
        std::cout << *(this->m_region_start + i) << " , ";
      }
    };
  };
}; // namespace mlib
