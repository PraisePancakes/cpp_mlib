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
#define _AMORT_FAC 2

/**
 *  @module
 *    this header implements a dynamic array structure, vector.
 *  This vector supports any derived iterator (see miterator.hpp) as well as any explicitly allocator (see mallocator.hpp).
 *  The purpose of avoiding a "baked-in" allocator/iterator was to enforce the SOC principle, this policy-like design allows for any derived iterator/allocator to
 *  ensure extensibility, separation of algorithms is crucial to this library.
 *
 * @internal module note
 *    To strictly follow a policy-like design, make use of allocator_traits and iterator_traits to enforce their respective algorithms / types respectively.
 *
 */

namespace mlib
{
  /**
   *  @class
   *  vec_base<T, alloc>
   *  this class is a trivial implementation helper that forwards the allocators underlying pointer and is restricted to derive a vector container.
   *  @template
   *  @param
   *  T
   *  represents the underlying container's value_type, this is used to forward to an implicit allocator.
   *  @param
   *  _Alloc
   *  this temp initializes a default allocator if no arguments were given.
   *
   */

  template <typename T, class _Alloc = allocator<T>>
  struct vec_base
  {
  protected:
    typedef _Alloc allocator_type;
    typedef typename allocator_traits<T>::pointer pointer;
    allocator_type _M_alloc;
    pointer _M_region_start;
    pointer _M_region_end;
    pointer _M_region_capacity;

    vec_base() : _M_alloc(nullptr), _M_region_start(nullptr), _M_region_end(nullptr), _M_region_capacity(nullptr) {};

    vec_base(size_t _capacity_)
    {
      _M_region_start = _M_alloc.allocate(_capacity_);
      _M_region_end = _M_region_start;
      _M_region_capacity = _M_region_start + _capacity_;
    }

    vec_base(_Alloc _allocator_, size_t _capacity_)
    {
      this->_M_alloc = _allocator_;
      _M_region_start = _M_alloc;
      _M_region_end = _M_region_start;
      _M_region_capacity = _M_region_start + _capacity_;
    };
    void _reserve(size_t _capacity_)
    {
      _M_region_start = _M_alloc.allocate(_capacity_);
      if (!_M_region_start)
      {
        return;
      }
      _M_region_end = _M_region_start;
      _M_region_capacity = _M_region_start + _capacity_;
    };

    void _resize_by_offset(const size_t _capacity_size_offset_)
    {
      size_t current_size = _size();
      size_t current_capacity = _capacity();
      size_t new_capacity = current_capacity + _capacity_size_offset_;
      pointer new_start = this->_M_alloc.reallocate(this->_M_region_start, new_capacity);

      if (!new_start)
      {
        throw std::bad_alloc();
      }

      if (new_start != this->_M_region_start)
      {
        this->_M_region_start = new_start;
        this->_M_region_end = new_start + current_size;
        this->_M_region_capacity = new_start + new_capacity;
      }
    }

    size_t _calculate_amortized_growth()
    {
      return ((_capacity() + 1) << 1);
    };

    size_t _size() const
    {
      return _M_region_end - _M_region_start;
    };

    size_t _capacity() const
    {
      return _M_region_capacity - _M_region_start;
    }

    vec_base(const vec_base &_other_) {

    };
  };

  template <typename T, class _Alloc = allocator<T>>
  class vec : protected vec_base<T, _Alloc>
  {
    typedef T value_type;
    typedef allocator_traits<T> allocator_traits;

  public:
    typedef typename allocator_traits::pointer pointer;
    typedef typename allocator_traits::const_pointer const_pointer;
    typedef typename allocator_traits::const_reference const_reference;
    typedef typename allocator_traits::reference reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef mlib::normal_iterator<pointer> iterator;
    typedef mlib::normal_iterator<const pointer> const_iterator;
    typedef mlib::reverse_iterator<pointer> reverse_iterator;
    typedef mlib::reverse_iterator<const pointer> const_reverse_iterator;

  private:
  public:
    vec() : vec_base<T, _Alloc>(0) {

            };
    vec(size_type _n_) : vec_base<T, _Alloc>(_n_) {

                         };
    vec(const vec_base<value_type> _other_) : vec_base<T, _Alloc>(_other_.size())
    {
    }
    vec(const vec<value_type> &_other_) : vec_base<T, _Alloc>(_other_.size())
    {
    }

    bool reserve(size_type _n_)
    {
      this->_reserve(_n_);
      return this->_M_region_start;
    };

    explicit vec(size_type _n_, const value_type &_v_) : vec_base<T, _Alloc>(_n_)
    {

      for (size_type i = 0; i < _n_; i++)
      {

        push_back(_v_);
      }
    }

    vec &operator=(const vec &_other_)
    {

      if (this != &_other_)
      {
        _deep_copy(_other_);
      }
      return *this;
    }
    /*
        mlib::vec<int> v{2, 3, 4, 5};

        v.insert(2, 5);

    */
    void insert(size_type _i_, const_reference _v_)
    {
      if (_i_ >= size())
      {
        push_back(_v_);
      }

      size_type si = _i_;
      size_type e = size();

      while (e > si)
      {
        *(this->_M_region_start + e) = *(this->_M_region_start + (e - 1));
        e--;
      }

      *(this->_M_region_start + e) = _v_;
      this->_M_region_end++;
    };

    void splice(size_type _s_, size_type _n_deletes_, std::initializer_list<value_type> _elems_)
    {
      const size_type start = _s_;
      size_type span = _n_deletes_;

      if (span > _elems_.size())
      {
        size_type i = start;
        for (size_type index = 0; index < _elems_.size(); index++)
        {
          *(this->_M_region_start + i) = _elems_.begin()[index];
          i++;
        }

        size_type _n_lshift = span - _elems_.size();

        i = start + _elems_.size();

        for (size_type iter = 0; iter < _n_lshift; iter++)
        {
          for (size_type j = start + _elems_.size(); j < this->size(); j++)
          {

            this->_M_region_start[j] = this->_M_region_start[j + 1];
          }

          this->_M_region_end--;
        }
      }
      else if (span < _elems_.size())
      {

        size_type list_cursor = 0;
        size_type i = start;
        for (; i < start + span; ++i)
        {
          this->_M_region_start[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }

        for (size_type j = i; j < start + _elems_.size(); j++)
        {

          insert(j, _elems_.begin()[list_cursor++]);
        }
      }
      else if (span == _elems_.size())
      {
        size_type list_cursor = 0;
        for (size_type i = start; i < start + span; ++i)
        {
          this->_M_region_start[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }
      }
    };

    vec(std::initializer_list<value_type> _elems_) : vec_base<T, _Alloc>(_elems_.size())
    {

      for (auto it = _elems_.begin(); it != _elems_.end(); it++)
      {
        this->_M_alloc.construct(this->_M_region_start + size(), *it);

        this->_M_region_end++;
      }
    };

    explicit vec(const allocator<value_type> &_alloc_, size_type _n_) : vec_base<T, _Alloc>(_alloc_, _n_) {

                                                                        };

    reference operator[](size_type _ptr_index_) const
    {
      return *(this->_M_region_start + _ptr_index_);
    };

    reference &at(size_type _ptr_index_) const
    {
      // bound check
      size_type max_value = std::numeric_limits<size_type>::max();

      if (_ptr_index_ >= size() || _ptr_index_ == max_value)
      {
        throw std::out_of_range("\nIndex succeeded bound with overflowed/underflowed range index, out of range ");
      }

      return *(this->_M_region_start + _ptr_index_);
    };
    // void(*_functor_)(reference)
    void for_each(void (*_functor_)(reference)) noexcept
    {
      for (size_type i = 0; i < size(); i++)
      {
        _functor_(*(this->_M_region_start + i));
      }
    };

    bool empty() const
    {
      return size() == 0;
    }

    void push_back(const_reference _v_)
    {
      if (size() >= capacity())
      {
        this->_resize_by_offset(this->_calculate_amortized_growth());
      }

      this->_M_alloc.construct(this->_M_region_start + size(), _v_);
      this->_M_region_end++;
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
      if (size() >= capacity())
      {
        this->_resize_by_offset(this->_calculate_amortized_growth());
      }

      this->_M_alloc.construct(this->_M_region_start + size(), _v_);
      this->_M_region_end++;
    }

    void push_back(std::initializer_list<value_type> _args_)
    {

      for (auto it = _args_.begin(); it != _args_.end(); ++it)
      {
        push_back(*it);
      }
    }

    void
    clear()
    {

      for (size_type i = 0; i < size(); i++)
      {
        this->_M_alloc.destroy(this->_M_region_start + i);
      }

      if (this->_M_region_start)
      {
        this->_M_alloc.deallocate(this->_M_region_start);
      }
      this->_M_region_end = this->_M_region_start;
    }

    iterator mbegin() const
    {
      return iterator(this->_M_region_start);
    }

    reverse_iterator mrbegin() const
    {
      return reverse_iterator(this->_M_region_end - 1);
    }

    reverse_iterator mrend() const
    {
      return reverse_iterator(this->_M_region_start - 1);
    }

    iterator mend() const
    {
      return iterator(this->_M_region_end);
    }

    bool contains(value_type _v_)
    {
      for (size_type i = 0; i < size(); i++)
      {
        if (*(this->_M_region_start + i) == _v_)
        {
          return true;
        }
      }
      return false;
    };

    // splice , insert
    void reverse()
    {
      // reverse whole vector
      if (this->size() == 0)
        return;

      if (this->size() == 1)
        return;

      size_type j = this->size() - 1;
      for (size_type i = 0; i < j; i++)
      {
        const value_type temp = this->_M_region_start[i];
        this->_M_region_start[i] = this->_M_region_start[j];
        this->_M_region_start[j] = temp;
        j--;
      }
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
        const value_type temp = this->_M_region_start[i];
        this->_M_region_start[i] = this->_M_region_start[j];
        this->_M_region_start[j] = temp;
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

      if (__end__ >= size())
      {
        __end__ = size();
      }

      if (__start__ > __end__)
      {
        __end__ = temp_start;
        __start__ = temp_end;
      }

      mlib::vec<value_type> v(__end__ - __start__);

      for (size_type i = __start__; i < __end__; i++)
      {
        this->_M_alloc.construct(v._M_region_start + v.size(), *(this->_M_region_start + i));
        v._M_region_end++;
      }

      return v;
    }

    inline size_type capacity() const
    {
      return this->_capacity();
    }

    void pop_back() noexcept
    {
      if (empty())
      {
        return;
      }
      this->_M_alloc.destroy(this->_M_region_start + size());
      this->_M_region_end--;
    };

    inline size_type size() const
    {
      return this->_size();
    }

    void print()
    {
      for (size_type i = 0; i < size(); i++)
      {
        std::cout << *(this->_M_region_start + i) << " , ";
      }
    };
  };
}; // namespace mlib
