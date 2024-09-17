#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <exception>
#include <initializer_list>
#include "miterator.hpp"
#include "mallocator.hpp"

#define _INITIAL_VECTOR_CAPACITY_ 1

inline ptrdiff_t address_diff_in_bytes(const void *addr1, const void *addr2)
{
  return std::abs(static_cast<const char *>(addr1) -
                  static_cast<const char *>(addr2));
}

namespace mlib
{
  template <typename _CTy, class _Alloc = allocator<_CTy>>
  class vec
  {
  private:
    _CTy *_M_container;
    _Alloc _M_allocator;
    size_t _M_dynamic_cursor;

    void _Vec_init_container(size_t _size_)
    {
      _M_dynamic_cursor = 0;
      this->_M_container = _M_allocator.allocate(_size_);
    }

    void _Vec_destruct_at(size_t i)
    {
      _M_container[i].~_CTy();
    }

    void _Vec_deep_copy(const vec &_other_)
    {
      _Vec_init_container(_other_.size());
      size_t safe_index = 0;
      for (size_t i = 0; i < _other_.size(); i++)
      {
        _M_container[safe_index] = _other_[i];
        safe_index++;
        _M_dynamic_cursor++;
      }
    };

    void _Vec_resize_capacity(const size_t _capacity_size_offset_)
    {
      const size_t new_size = _M_allocator._Alloc_capacity_size + _capacity_size_offset_;
      this->_M_container = _M_allocator.reallocate(new_size);
    };

  public:
    typedef iterator<_CTy> iterator;
    typedef const_reverse_iterator<_CTy> const_reverse_iterator;
    typedef const_iterator<_CTy> const_iterator;
    typedef reverse_iterator<_CTy> reverse_iterator;

    iterator mbegin() const
    {
      return iterator(_M_container);
    }

    iterator mend() const
    {
      return iterator(_M_container + _M_dynamic_cursor);
    }

    const_iterator mcbegin() const
    {
      return const_iterator(_M_container);
    }

    const_iterator mcend() const
    {
      return const_iterator(_M_container + _M_dynamic_cursor);
    }

    reverse_iterator mrbegin() const
    {
      return reverse_iterator(_M_container + (_M_dynamic_cursor - 1));
    }

    reverse_iterator mrend() const
    {
      return reverse_iterator(_M_container - 1);
    }

    const_reverse_iterator mcrbegin() const
    {
      return const_reverse_iterator(_M_container + (_M_dynamic_cursor - 1));
    }

    const_reverse_iterator mcrend() const
    {
      return const_reverse_iterator(_M_container - 1);
    }

    vec(const size_t &_container_size_)
    {
      _Vec_init_container(_container_size_);
    };

    vec() { _Vec_init_container(_INITIAL_VECTOR_CAPACITY_); };
    bool operator==(const vec &_other_)
    {
      if (this->size() != _other_.size())
        return false;

      size_t other_cursor = 0;
      for (size_t i = 0; i < this->size(); i++)
      {
        if (_M_container[i] != _other_.at(other_cursor))
          return false;
        other_cursor++;
      }

      return true;
    };
    vec(std::initializer_list<_CTy> _elems_)
    {

      _Vec_init_container(_elems_.size());
      std::cout << _elems_.size() << " : " << _M_allocator._Alloc_capacity_size << std::endl;

      size_t index = 0;
      for (auto it = _elems_.begin(); it != _elems_.end(); it++)
      {
        *(_M_container + index) = *it;
        index++;
        _M_dynamic_cursor++;
      }
    };
    vec(const vec &_other_) { _Vec_deep_copy(_other_); };
    vec &operator=(const vec &_other_)
    {
      if (this != &_other_)
      {
        clear();
        _Vec_deep_copy(_other_);
      }
      return *this;
    }
    // api
    _CTy &operator[](const size_t _ptr_index_) const
    {
      return *(_M_container + _ptr_index_);
    };

    /*
        @brief
            decrement the exclusive cursor by one to index into the final element.
          dereference the element and call its destructor

        #note
            on primitives, the destructor is a no-op which means we are
       sacrificing O(n) bytes, n being the pop length. TO:DO incorporate aligned
       storage
    */

    _CTy &at(int _ptr_index_) const
    {
      // bound check

      if (_ptr_index_ < 0)
      {

        throw std::out_of_range("\nIndex succeeded left bound with underflowed range index, out of range");
      }
      if (_ptr_index_ >= _M_dynamic_cursor)
      {
        throw std::out_of_range("\nIndex succeeded right bound with overflowed range index, out of range ");
      }

      return *(_M_container + _ptr_index_);
    };

    bool empty() const
    {
      return _M_dynamic_cursor == 0;
    }

    _CTy *pop_back() noexcept
    {
      if (empty())
      {
        return nullptr;
      }
      _M_dynamic_cursor--;
      return ((_M_dynamic_cursor + 1) + _M_container);
    };

    void for_each(std::function<void(_CTy)> _functor_) noexcept
    {
      for (size_t i = 0; i <= _M_dynamic_cursor; i++)
      {
        _functor_(*(_M_container + i));
      }
    };

    void clear()
    {

      for (auto it = mbegin(); it != mend(); ++it)
      {
        (*it).~_CTy();
      }

      if (_M_container)
      {

        free(_M_container);
        _M_container = nullptr;
      }
      _M_dynamic_cursor = 0;
    }

    void splice(size_t _start_, size_t _num_deletions_, std::initializer_list<_CTy> _args_list_)
    {
      const size_t span = _num_deletions_;
      const size_t args_size = _args_list_.size();

      if (span > args_size)
      {

        size_t list_cursor = 0;

        for (size_t i = _start_; i < _start_ + args_size; i++)
        {
          _M_container[i] = _args_list_.begin()[list_cursor];
          list_cursor++;
        }
        //    1        3     c
        //{0, 4, 5, 6, 7, 8};
        //{7 , 8}
        const size_t lshift_iterations = span - args_size;

        for (size_t i = 0; i < lshift_iterations; i++)
        {
          for (size_t j = _start_ + args_size; j < _M_dynamic_cursor; j++)
          {
            _M_container[j] = _M_container[j + 1];
          }
          _M_dynamic_cursor--;
        }

        // fill left to fill in deletions
      }
      else if (span < args_size)
      {
        size_t i = _start_;
        size_t list_cursor = 0;

        for (i = _start_; i < _start_ + span; ++i)
        {
          _M_container[i] = _args_list_.begin()[list_cursor];
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
          _M_container[i] = _args_list_.begin()[list_cursor];
          list_cursor++;
        }
      }
    }

    /*
     @brief
       slices vector into another vector, returns deep-copy of new sliced vector
   */
    mlib::vec<_CTy> slice(size_t _start_, size_t _end_)
    {
      const int temp_start = _start_;
      const int temp_end = _end_;

      if (_start_ < 0)
      {
        _start_ = 0;
      }

      if (_end_ >= _M_dynamic_cursor)
      {
        _end_ = _M_dynamic_cursor;
      }

      if (_start_ > _end_)
      {
        _end_ = temp_start;
        _start_ = temp_end;
      }

      mlib::vec<_CTy> v(_end_ - _start_);

      for (size_t i = _start_; i < _end_; i++)
      {
        v.push_back(_M_container[i]);
      }

      return std::move(v);
    }

    //{0, 1, 2, 3, 4, 5};
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
        const _CTy temp = _M_container[i];
        _M_container[i] = _M_container[j];
        _M_container[j] = temp;
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
        const _CTy temp = _M_container[i];
        _M_container[i] = _M_container[j];
        _M_container[j] = temp;
        j--;
      }
    };

    // insert non modifiable lvalue or rvalue
    void insert(size_t index, const _CTy &value)
    {
      _Vec_resize_capacity(1);

      size_t shift_index = _M_dynamic_cursor;

      while (shift_index > index)
      {

        // [1, 4, ]
        _CTy *temp = _M_container + shift_index;
        *(temp) = *(temp - 1);
        shift_index--;
      }

      *(_M_container + index) = value;
      _M_dynamic_cursor++;
    };

    size_t size() const
    {
      return _M_dynamic_cursor;
    }
    size_t cursor_byte_size() const noexcept
    {
      return _M_dynamic_cursor * _M_allocator._Alloc_cell_size;
    };
    size_t capacity_byte_size() const noexcept
    {
      return _M_allocator._Alloc_capacity_byte_size;
    }
    void push_back(const _CTy &_val_)
    {
      if (_M_dynamic_cursor >= _M_allocator._Alloc_capacity_size)
      {
        // realloc
        _M_container = _M_allocator.reallocate(_M_allocator._Alloc_capacity_size = (_M_allocator._Alloc_capacity_size * 2));
      }

      *(_M_container + _M_dynamic_cursor) = _val_;
      _M_dynamic_cursor++;
    };

    size_t capacity() const
    {
      return this->_M_allocator._Alloc_capacity_size;
    }

    ~vec()
    {
      clear();
    };
  }; // end class vec
} // namespace mlib
