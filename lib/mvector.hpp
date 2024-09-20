#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <exception>
#include <initializer_list>
#include <cstddef>
#include "mallocator.hpp"

#define _DEF_VECTOR_CAPACITY_ 1
#define _VECTOR_AMORT_GROWTH_FACTOR 2

namespace mlib
{
  template <typename _Ty, class _Alloc = allocator<_Ty>>
  class vec
  {
    typedef _Ty value_type;
    typedef _Alloc allocator_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;

    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;

    allocator_type _M_allocator;

    size_type _M_capacity;
    pointer _M_begin;
    size_type _M_size;

    void _init_container(size_type _n_ = 0)
    {
      _M_size = 0;
      _M_capacity = 0;
      _M_begin = nullptr;
    }

    void _init_container(const allocator<value_type> &_alloc_, size_t _n_ = 0)
    {
      if (_n_ == 0)
      {
        _n_ = _DEF_VECTOR_CAPACITY_;
        _M_size = 0;
      }
      else
      {
        _M_size = _n_;
      }

      _M_capacity = _n_ * _VECTOR_AMORT_GROWTH_FACTOR;
      pointer region_start = _alloc_.allocate(_M_capacity);
      _M_begin = region_start;
      region_start = nullptr;
    }

    void _resize(const size_t _capacity_size_offset_)
    {
      const size_t new_size = _M_capacity + _capacity_size_offset_;
      this->_M_begin = _M_allocator.reallocate(_M_begin, new_size);
    };

    void _deep_copy(const vec &_other_)
    {
      _init_container(_other_.size());
      size_t safe_idx = 0;
      for (size_type i = 0; i < size(); i++)
      {
        _M_allocator.construct(_M_begin + i, _other_[safe_idx]);
      }
    }

  public:
    explicit vec(size_type _n_, const value_type &_v_)
    {
      _init_container(_n_);

      for (size_type i = 0; i < size(); i++)
      {
        _M_allocator.construct(_M_begin + i, _v_);
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

    vec(std::initializer_list<value_type> _elems_)
    {

      _init_container(_elems_.size());

      size_t index = 0;
      for (auto it = _elems_.begin(); it != _elems_.end(); it++)
      {
        _M_allocator.construct(_M_begin + index, *it);
        index++;
      }
    };

    reference operator[](difference_type _ptr_index_) const
    {
      return *(_M_begin + _ptr_index_);
    };

    reference &at(difference_type _ptr_index_) const
    {
      // bound check
      if (_ptr_index_ < 0)
      {
        throw std::out_of_range("\nIndex succeeded left bound with underflowed range index, out of range");
      }
      if (_ptr_index_ >= _M_size)
      {
        throw std::out_of_range("\nIndex succeeded right bound with overflowed range index, out of range ");
      }

      return *(_M_begin + _ptr_index_);
    };

    explicit vec(const allocator<value_type> &_alloc_)
    {
      _init_container(_alloc_);
    };

    explicit vec(const allocator<value_type> &_alloc_, size_type _n_)
    {
      _init_container(_alloc_, _n_);
    };

    vec()
    {
      _init_container();
    };

    vec(size_t _n_)
    {
      _init_container(_n_);
    };

    void for_each(std::function<void(reference)> _functor_) noexcept
    {
      for (size_t i = 0; i < _M_size; i++)
      {
        _functor_(*(_M_begin + i));
      }
    };

    bool empty() const
    {
      return _M_size == 0;
    }

    void push_back(const_reference _v_)
    {

      if (size() >= _M_capacity)
      {
        _M_capacity = (_M_capacity + 1) * _VECTOR_AMORT_GROWTH_FACTOR;
        _M_begin = _M_allocator.reallocate(_M_begin, _M_capacity);
      }

      _M_allocator.construct(_M_begin + (_M_size), _v_);
      _M_size++;
    }

    /*
      @function
        push_back

      @args
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

      if (size() >= _M_capacity)
      {
        _M_capacity = (_M_capacity + 1) * _VECTOR_AMORT_GROWTH_FACTOR;
        _M_begin = _M_allocator.reallocate(_M_begin, _M_capacity);
      }

      _M_allocator.construct(_M_begin + (_M_size), _v_);
      _M_size++;
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

      for (size_t i = 0; i < _M_size; i++)
      {
        _M_allocator.destroy(_M_begin + i);
      }

      if (_M_begin)
      {
        _M_allocator.deallocate(_M_begin);
      }
      _M_size = 0;
    }

    void insert(size_t index, const_reference value)
    {

      _resize(1);

      size_t shift_index = _M_size;

      while (shift_index > index)
      {

        // [1, 4, ]
        pointer temp = _M_begin + shift_index;
        *(temp) = *(temp - 1);
        shift_index--;
      }

      *(_M_begin + index) = value;
      _M_size++;
    };

    void splice(size_t _start_, size_t _num_deletions_, std::initializer_list<value_type> _args_list_)
    {
      const size_t span = _num_deletions_;
      const size_t args_size = _args_list_.size();

      if (span > args_size)
      {

        size_t list_cursor = 0;

        for (size_t i = _start_; i < _start_ + args_size; i++)
        {
          _M_begin[i] = _args_list_.begin()[list_cursor];
          list_cursor++;
        }
        //    1        3     c
        //{0, 4, 5, 6, 7, 8};
        //{7 , 8}
        const size_t lshift_iterations = span - args_size;

        for (size_t i = 0; i < lshift_iterations; i++)
        {
          for (size_t j = _start_ + args_size; j < _M_size; j++)
          {
            _M_begin[j] = _M_begin[j + 1];
          }
          _M_size--;
        }

        // fill left to fill in deletions
      }
      else if (span < args_size)
      {
        size_t i = _start_;
        size_t list_cursor = 0;

        for (i = _start_; i < _start_ + span; ++i)
        {
          _M_begin[i] = _args_list_.begin()[list_cursor];
          list_cursor++;
        }

        for (size_t j = i; j < _start_ + args_size; j++)
        {

          insert(j, _args_list_.begin()[list_cursor++]);
        }
      }
      else if (span == args_size)
      {
        size_t list_cursor = 0;
        for (size_t i = _start_; i < _start_ + span; ++i)
        {
          _M_begin[i] = _args_list_.begin()[list_cursor];
          list_cursor++;
        }
      }
    }

    void reverse()
    {
      // reverse whole vector
      if (this->size() == 0)
        return;

      if (this->size() == 1)
        return;

      size_t j = this->size() - 1;
      for (size_t i = 0; i < j; i++)
      {
        const value_type temp = _M_begin[i];
        _M_begin[i] = _M_begin[j];
        _M_begin[j] = temp;
        j--;
      }
    };

    void reverse(size_t _start_, size_t _end_)
    {
      // reverse from start to end.
      if (this->size() == 0)
        return;

      if (this->size() == 1)
        return;

      const size_t temp_start = _start_;
      const size_t temp_end = _end_;

      if (_start_ > _end_)
      {
        _start_ = temp_end;
        _end_ = temp_start;
      }

      size_t j = _end_;
      for (size_t i = _start_; i < j; i++)
      {
        const value_type temp = _M_begin[i];
        _M_begin[i] = _M_begin[j];
        _M_begin[j] = temp;
        j--;
      }
    };

    mlib::vec<value_type> slice(size_t __start__, size_t __end__)
    {
      const int temp_start = __start__;
      const int temp_end = __end__;

      if (__start__ < 0)
      {
        __start__ = 0;
      }

      if (__end__ >= _M_size)
      {
        __end__ = _M_size;
      }

      if (__start__ > __end__)
      {
        __end__ = temp_start;
        __start__ = temp_end;
      }

      mlib::vec<value_type> v(__end__ - __start__);

      size_type index = 0;
      for (size_t i = __start__; i < __end__; i++)
      {
        allocator_traits<value_type>::construct(v._M_begin + index, *(_M_begin + i));
      }

      return v;
    }

    // mlib::vec<value_type> slice(size_t __start__, size_t __end__)
    // {
    //   const int temp_start = __start__;
    //   const int temp_end = __end__;

    //   if (__start__ < 0)
    //   {
    //     __start__ = 0;
    //   }

    //   if (__end__ >= _M_size)
    //   {
    //     __end__ = _M_size;
    //   }

    //   if (__start__ > __end__)
    //   {
    //     __end__ = temp_start;
    //     __start__ = temp_end;
    //   }

    //   mlib::vec<value_type> v(__end__ - __start__);

    //   size_type index = 0;
    //   for (size_t i = __start__; i < __end__; i++)
    //   {
    //     allocator_traits<value_type>::construct(v._M_begin + index, *(_M_begin + i));
    //   }

    //   return v;
    // }

    size_t capacity() const
    {
      return this->_M_capacity;
    }

    void pop_back() noexcept
    {
      if (empty())
      {
        return;
      }
      _M_allocator.destroy(_M_begin + _M_size);
      _M_size--;
    };

    size_type size() const
    {
      return _M_size;
    }

    void print()
    {
      for (size_type i = 0; i < size(); i++)
      {
        std::cout << *(_M_begin + i) << " , ";
      }
    };
  };
}; // namespace mlib
