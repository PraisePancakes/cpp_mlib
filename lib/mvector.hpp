#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <exception>
#include <initializer_list>
#include <cstddef>
#include "mallocator.hpp"
#include "miterator.hpp"

#define _DEF_VECTOR_CAPACITY_ 1
#define _VECTOR_AMORT_GROWTH_FACTOR 2

namespace mlib
{

  template <typename _Ty, class _Alloc = allocator<_Ty>>
  struct vec_base
  {
    typedef typename allocator_traits<_Ty>::pointer pointer;

    struct impl_data
    {
      pointer _M_region_start;
      pointer _M_region_end;
      pointer _M_region_capacity;

      impl_data() : _M_region_capacity(nullptr), _M_region_end(nullptr), _M_region_start(nullptr) {};
      impl_data(pointer _s_, pointer _e_, pointer _c_) : _M_region_start(_s_), _M_region_capacity(_c_), _M_region_end(_e_) {};
      impl_data(impl_data &&_other_) : _M_region_start(_other_._M_region_start),
                                       _M_region_end(_other_._M_region_end),
                                       _M_region_capacity(_other_._M_region_capacity)
      {
        _other_._M_region_capacity = _other_._M_region_start = _other_._M_region_end = pointer();
      }

      impl_data(const impl_data &_other_)
      {
        _M_region_capacity = _other_._M_region_capacity;
        _M_region_end = _other_._M_region_end;
        _M_region_start = _other_._M_region_start;
      }

      impl_data &operator=(const impl_data &_other_)
      {
        _M_region_capacity = _other_._M_region_capacity;
        _M_region_end = _other_._M_region_end;
        _M_region_start = _other_._M_region_start;
        return *this;
      }

      void _copy_data(const impl_data &_other_)
      {
        _M_region_capacity = _other_._M_region_capacity;
        _M_region_end = _other_._M_region_end;
        _M_region_start = _other_._M_region_start;
      };

      void _copy_data(pointer _s_, pointer _c_)
      {
        if (_s_ == nullptr)
        {
          std::cout << "null";
        }

        if (_c_ == nullptr)
        {
          std::cout << "null";
        }

        _M_region_start = _s_;
        _M_region_end = _s_;
        _M_region_capacity = _c_;
      };

      ~impl_data()
      {
        _M_region_start = nullptr;
        _M_region_capacity = nullptr;
        _M_region_end = nullptr;
      };
    };

    impl_data _M_impl;

    vec_base() {}

    vec_base(const vec_base &_other_)
    {
      this->_M_impl = _other_._M_impl;
    };

    vec_base &operator=(const vec_base &_other_)
    {
      this->_M_impl._copy_data(_other_._M_impl);
      return *this;
    }
  };

  template <typename _Ty, class _Alloc = allocator<_Ty>>
  class vec : protected vec_base<_Ty, _Alloc>
  {
    typedef _Ty value_type;
    typedef vec_base<_Ty, _Alloc> base;
    typedef _Alloc allocator_type;
    typedef allocator_traits<_Ty> allocator_traits;

  public:
    typedef typename base::pointer pointer;
    typedef typename allocator_traits::const_pointer const_pointer;
    typedef typename allocator_traits::const_reference const_reference;
    typedef typename allocator_traits::reference reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef mlib::normal_iterator<pointer> iterator;
    typedef mlib::normal_iterator<const_pointer> const_iterator;

  private:
    typename base::impl_data _M_impl; // retreive implementation iterators
    base _M_base;

    void _init_container(difference_type _n_ = 0)
    {
      if (_n_ == 0)
      {
        _n_ = _DEF_VECTOR_CAPACITY_;
      }

      difference_type capacity = _n_ * _VECTOR_AMORT_GROWTH_FACTOR;
      pointer start = allocator_traits::allocate(capacity);
      _M_impl._copy_data(start, start + capacity);
    }

    void _init_container(const vec_base<value_type> &_other_)
    {

      this->_M_impl = _other_._M_impl;
    }

    void _resize_by_offset(const difference_type _capacity_size_offset_)
    {

      _M_impl._M_region_capacity = _M_impl._M_region_capacity + _capacity_size_offset_;
      std::cout << capacity() << std::endl;
      _M_impl._M_region_start = allocator_traits::reallocate(_M_impl._M_region_start, _M_impl._M_region_capacity - _M_impl._M_region_start);
    };

    void _deep_copy(const vec &_other_)
    {
      _init_container(_other_.size());
      size_t safe_idx = 0;
      for (size_type i = 0; i < size(); i++)
      {
        allocator_traits::construct(_M_impl._M_region_start + i, _other_[safe_idx]);
      }
    }

  public:
    vec()
    {
      _init_container();
    };

    vec(difference_type _n_)
    {
      _init_container(_n_);
    };

    vec(const vec_base<value_type> _other_)
    {
      this->_init_container(_other_);
      this->_M_base = _other_;
    }

    vec(const vec<value_type> &_other_)
    {
      this->_M_impl = _other_._M_impl;
      this->_M_base = _other_._M_base;
    }

    explicit vec(size_type _n_, const value_type &_v_)
    {
      _init_container(_n_);

      for (size_type i = 0; i < size(); i++)
      {
        allocator_traits::construct(_M_impl._M_region_start + i, _v_);
        _M_impl._M_region_end++;
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
    void insert(difference_type _i_, const_reference _v_)
    {
      if (_i_ >= size())
      {
        push_back(_v_);
      }

      difference_type si = _i_;
      difference_type e = size();

      while (e > si)
      {
        *(_M_impl._M_region_start + e) = *(_M_impl._M_region_start + (e - 1));
        e--;
      }

      *(_M_impl._M_region_start + e) = _v_;
      _M_impl._M_region_end++;
    };

    void splice(difference_type _s_, difference_type _n_deletes_, std::initializer_list<value_type> _elems_)
    {
      const difference_type start = _s_;
      difference_type span = _n_deletes_;

      if (span > _elems_.size())
      {
        difference_type i = start;
        for (difference_type index = 0; index < _elems_.size(); index++)
        {
          *(_M_impl._M_region_start + i) = _elems_.begin()[index];
          i++;
        }

        difference_type _n_lshift = span - _elems_.size();

        i = start + _elems_.size();

        for (difference_type iter = 0; iter < _n_lshift; iter++)
        {
          for (size_t j = start + _elems_.size(); j < this->size(); j++)
          {

            _M_impl._M_region_start[j] = _M_impl._M_region_start[j + 1];
          }

          _M_impl._M_region_end--;
        }
      }
      else if (span < _elems_.size())
      {

        size_t list_cursor = 0;
        size_t i = start;
        for (; i < start + span; ++i)
        {
          _M_impl._M_region_start[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }

        for (size_t j = i; j < start + _elems_.size(); j++)
        {

          insert(j, _elems_.begin()[list_cursor++]);
        }
      }
      else if (span == _elems_.size())
      {
        size_type list_cursor = 0;
        for (size_type i = start; i < start + span; ++i)
        {
          _M_impl._M_region_start[i] = _elems_.begin()[list_cursor];
          list_cursor++;
        }
      }
    };

    vec(std::initializer_list<value_type> _elems_)
    {

      _init_container(_elems_.size());

      for (auto it = _elems_.begin(); it != _elems_.end(); it++)
      {
        allocator_traits::construct(_M_impl._M_region_start + size(), *it);

        _M_impl._M_region_end++;
      }
    };

    explicit vec(const allocator<value_type> &_alloc_)
    {
      _init_container(_alloc_);
    };

    explicit vec(const allocator<value_type> &_alloc_, size_type _n_)
    {
      _init_container(_alloc_, _n_);
    };

    reference operator[](difference_type _ptr_index_) const
    {
      return *(_M_impl._M_region_start + _ptr_index_);
    };

    reference &at(size_type _ptr_index_) const
    {
      // bound check
      if (_ptr_index_ < 0)
      {
        throw std::out_of_range("\nIndex succeeded left bound with underflowed range index, out of range");
      }
      if (_ptr_index_ >= size())
      {
        throw std::out_of_range("\nIndex succeeded right bound with overflowed range index, out of range ");
      }

      return *(_M_impl._M_region_start + _ptr_index_);
    };

    void for_each(std::function<void(reference)> _functor_) noexcept
    {
      for (size_t i = 0; i < size(); i++)
      {
        _functor_(*(_M_impl._M_region_start + i));
      }
    };

    bool empty() const
    {
      return size() == 0;
    }

    void push_back(const_reference _v_)
    {

      if (_M_impl._M_region_end == _M_impl._M_region_capacity)
      {

        _M_impl._M_region_capacity = _M_impl._M_region_capacity + ((_M_impl._M_region_capacity - _M_impl._M_region_end) * _VECTOR_AMORT_GROWTH_FACTOR);
        _resize_by_offset(_M_impl._M_region_capacity - _M_impl._M_region_start);
      }

      allocator_traits::construct(_M_impl._M_region_start + (size()), _v_);
      _M_impl._M_region_end++;
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

      if (_M_impl._M_region_end == _M_impl._M_region_capacity)
      {
        _M_impl._M_region_capacity = (_M_impl._M_region_capacity + 1) * _VECTOR_AMORT_GROWTH_FACTOR;
        _M_impl._M_region_start = allocator_traits::reallocate(_M_impl._M_region_start, _M_impl._M_region_capacity - _M_impl._M_region_start);
      }

      allocator_traits::construct(_M_impl._M_region_start + (size()), _v_);

      _M_impl._M_region_end++;
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

      for (size_t i = 0; i < size(); i++)
      {
        allocator_traits::destroy(_M_impl._M_region_start + i);
      }

      if (_M_impl._M_region_start)
      {
        allocator_traits::deallocate(_M_impl._M_region_start);
      }
      _M_impl._M_region_end = _M_impl._M_region_start;
    }

    iterator mbegin() const
    {
      return iterator(_M_impl._M_region_start);
    }

    iterator mend() const
    {
      return iterator(_M_impl._M_region_end);
    }

    base &_base()
    {
      return _M_base;
    }
    // splice , insert
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
        const value_type temp = _M_impl._M_region_start[i];
        _M_impl._M_region_start[i] = _M_impl._M_region_start[j];
        _M_impl._M_region_start[j] = temp;
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
        const value_type temp = _M_impl._M_region_start[i];
        _M_impl._M_region_start[i] = _M_impl._M_region_start[j];
        _M_impl._M_region_start[j] = temp;
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

      for (size_t i = __start__; i < __end__; i++)
      {
        allocator_traits::construct(v._M_impl._M_region_start + v.size(), *(this->_M_impl._M_region_start + i));
        v._M_impl._M_region_end++;
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
    //     allocator_traits<value_type>::construct(v._M_impl._M_region_start + index, *(_M_impl._M_region_start + i));
    //   }

    //   return v;
    // }

    difference_type capacity() const
    {
      return (_M_impl._M_region_capacity - _M_impl._M_region_start);
    }

    void pop_back() noexcept
    {
      if (empty())
      {
        return;
      }
      allocator_traits::destroy(_M_impl._M_region_start + size());
      _M_impl._M_region_end--;
    };

    difference_type size() const
    {

      return (_M_impl._M_region_end - _M_impl._M_region_start);
    }

    void print()
    {
      for (size_type i = 0; i < size(); i++)
      {
        std::cout << *(_M_impl._M_region_start + i) << " , ";
      }
    };
  };
}; // namespace mlib
