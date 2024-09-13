#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
#include <exception>
#include <initializer_list>

#define __INITIAL_VECTOR_CAPACITY__ 1

inline ptrdiff_t address_diff_in_bytes(const void *addr1, const void *addr2)
{
  return std::abs(static_cast<const char *>(addr1) -
                  static_cast<const char *>(addr2));
}

namespace mlib
{
  template <typename __VECTYPE>
  class vec
  {
  private:
    __VECTYPE *_M_container;

    size_t _M_capacity_size;
    size_t _M_dynamic_cursor;
    size_t _M_cell_size;

    void _Vec_init_container(size_t __size__)
    {
      this->_M_capacity_size = __size__;
      _M_dynamic_cursor = 0;
      this->_M_cell_size = sizeof(__VECTYPE);
      this->_M_container = (__VECTYPE *)malloc(__size__ * _M_cell_size);
    }

    void _Vec_destruct_at(size_t i)
    {
      _M_container[i].~__VECTYPE();
    }

    void _Vec_deep_copy(const vec &__other__)
    {
      _Vec_init_container(__other__.size());
      size_t safe_iterator = 0;
      for (size_t i = 0; i < __other__.size(); i++)
      {
        _M_container[safe_iterator] = __other__[i];
        safe_iterator++;
        _M_dynamic_cursor++;
      }
    };

    void _Vec_resize_capacity(const size_t __capacity_size_offset__)
    {

      const size_t new_size =
          (_M_capacity_size + __capacity_size_offset__) * _M_cell_size;
      this->_M_container = (__VECTYPE *)realloc(_M_container, new_size);
      _M_capacity_size += __capacity_size_offset__;
    };

  public:
    class iterator
    {
      __VECTYPE *_Iterator_ptr;

    public:
      iterator() : _Iterator_ptr(nullptr) {}
      iterator(__VECTYPE *__iter_loc__) : _Iterator_ptr(__iter_loc__) {}

      bool operator!=(const iterator &other)
      {

        return _Iterator_ptr != other._Iterator_ptr;
      };

      iterator &operator++()
      {

        ++_Iterator_ptr;
        return *this;
      }
      iterator &operator--()
      {

        --_Iterator_ptr;
        return *this;
      }

      __VECTYPE &operator*() const
      {
        return *(_Iterator_ptr);
      };
      ~iterator() {};
    };

    class reverse_iterator
    {
      __VECTYPE *_Iterator_ptr;

    public:
      reverse_iterator() : _Iterator_ptr(nullptr) {};
      reverse_iterator(__VECTYPE *__iter_loc__) : _Iterator_ptr(__iter_loc__) {};
      reverse_iterator(const reverse_iterator &other) = delete;
      reverse_iterator(reverse_iterator &&other) = delete;

      bool operator!=(const reverse_iterator &other)
      {

        return _Iterator_ptr != other._Iterator_ptr;
      };

      reverse_iterator &operator++()
      {
        --_Iterator_ptr;
        return *this;
      }

      __VECTYPE &operator*()
      {
        return *(_Iterator_ptr);
      };

      reverse_iterator &operator--()
      {

        _Iterator_ptr++;
        return *this;
      }
    };

    class const_reverse_iterator
    {
      __VECTYPE *_Iterator_ptr;

    public:
      const_reverse_iterator() : _Iterator_ptr(nullptr) {};
      const_reverse_iterator(__VECTYPE *__iter_loc__) : _Iterator_ptr(__iter_loc__) {};
      const_reverse_iterator(const const_reverse_iterator &other) = delete;
      const_reverse_iterator(const_reverse_iterator &&other) = delete;

      bool operator!=(const const_reverse_iterator &other)
      {

        return _Iterator_ptr != other._Iterator_ptr;
      };

      const const_reverse_iterator &operator++()
      {
        --_Iterator_ptr;
        return *this;
      }

      const __VECTYPE &operator*()
      {
        return *(_Iterator_ptr);
      };

      const const_reverse_iterator &operator--()
      {

        _Iterator_ptr++;
        return *this;
      };
      ~const_reverse_iterator() {};
    };

    class const_iterator
    {
      __VECTYPE *_Iterator_ptr;

    public:
      const_iterator() : _Iterator_ptr(nullptr) {}
      const_iterator(__VECTYPE *__iter_loc__) : _Iterator_ptr(__iter_loc__) {}
      const_iterator(const const_iterator &other) = delete;
      const_iterator(const_iterator &&other) = delete;

      bool operator!=(const const_iterator &other)
      {

        return _Iterator_ptr != other._Iterator_ptr;
      };

      const const_iterator &operator++()
      {

        ++_Iterator_ptr;
        return *this;
      }

      const __VECTYPE &operator*() const
      {
        return *(_Iterator_ptr);
      };
      ~const_iterator() {};
    };

    vec(const size_t &__container_size__)
    {
      _Vec_init_container(__container_size__);
    };

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

    vec() { _Vec_init_container(__INITIAL_VECTOR_CAPACITY__); };
    bool operator==(const vec &__other__)
    {
      if (this->size() != __other__.size())
        return false;

      size_t other_cursor = 0;
      for (size_t i = 0; i < this->size(); i++)
      {
        if (_M_container[i] != __other__.at(other_cursor))
          return false;
        other_cursor++;
      }

      return true;
    };
    vec(std::initializer_list<__VECTYPE> __elems__)
    {

      _Vec_init_container(__elems__.size());

      size_t index = 0;
      for (auto it = __elems__.begin(); it != __elems__.end(); it++)
      {
        *(_M_container + index) = *it;
        index++;
        _M_dynamic_cursor++;
      }
    };
    vec(const vec &__other__) { _Vec_deep_copy(__other__); };
    vec &operator=(const vec &__other__)
    {
      if (this != &__other__)
      {
        free(_M_container);
        _Vec_deep_copy(__other__);
      }
      return *this;
    }
    // api
    __VECTYPE &operator[](const size_t __ptr_index__) const
    {
      return *(_M_container + __ptr_index__);
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

    __VECTYPE &at(int __ptr_index__) const
    {
      // bound check

      if (__ptr_index__ < 0)
      {

        throw std::out_of_range("\nIndex succeeded left bound with underflowed range index, out of range");
      }
      if (__ptr_index__ >= _M_dynamic_cursor)
      {
        throw std::out_of_range("\nIndex succeeded right bound with overflowed range index, out of range ");
      }

      return *(_M_container + __ptr_index__);
    };

    bool empty() const
    {
      return _M_dynamic_cursor == 0;
    }

    void pop_back() noexcept
    {
      if (_M_dynamic_cursor == 0)
      {
        return;
      }
      --_M_dynamic_cursor;
      (_M_container + _M_dynamic_cursor)->~__VECTYPE();
    };

    void for_each(std::function<void(__VECTYPE)> __functor__) noexcept
    {
      for (size_t i = 0; i < _M_capacity_size; i++)
      {
        __functor__(*(_M_container + i));
      }
    };

    void clear()
    {
      for (auto it = mbegin(); it != mend(); ++it)
      {
        (*it).~__VECTYPE();
      }

      if (_M_container)
      {
        free(_M_container);
        _M_container = nullptr;
      }
    }

    void splice(size_t __start__, size_t __num_deletions__, std::initializer_list<__VECTYPE> __args_list__)
    {
      const size_t span = __num_deletions__;
      const size_t args_size = __args_list__.size();

      if (span > args_size)
      {

        size_t list_cursor = 0;

        for (size_t i = __start__; i < __start__ + args_size; i++)
        {
          _M_container[i] = __args_list__.begin()[list_cursor];
          list_cursor++;
        }
        //    1        3     c
        //{0, 4, 5, 6, 7, 8};
        //{7 , 8}
        const size_t lshift_iterations = span - args_size;

        for (size_t i = 0; i < lshift_iterations; i++)
        {
          for (size_t j = __start__ + args_size; j < _M_dynamic_cursor; j++)
          {
            _M_container[j] = _M_container[j + 1];
          }
          _M_dynamic_cursor--;
        }

        // fill left to fill in deletions
      }
      else if (span < args_size)
      {
        size_t i = __start__;
        size_t list_cursor = 0;

        for (i = __start__; i < __start__ + span; ++i)
        {
          _M_container[i] = __args_list__.begin()[list_cursor];
          list_cursor++;
        }

        for (size_t j = i; j < __start__ + args_size; j++)
        {

          insert(j, __args_list__.begin()[list_cursor++]);
        }
      }
      else if (span == args_size)
      {
        size_t list_cursor = 0;
        for (size_t i = __start__; i < __start__ + span; ++i)
        {
          _M_container[i] = __args_list__.begin()[list_cursor];
          list_cursor++;
        }
      }
    }

    /*
     @brief
       slices vector into another vector, returns deep-copy of new sliced vector
   */
    mlib::vec<__VECTYPE> slice(size_t __start__, size_t __end__)
    {
      const int temp_start = __start__;
      const int temp_end = __end__;

      if (__start__ < 0)
      {
        __start__ = 0;
      }

      if (__end__ >= _M_dynamic_cursor)
      {
        __end__ = _M_dynamic_cursor;
      }

      if (__start__ > __end__)
      {
        __end__ = temp_start;
        __start__ = temp_end;
      }

      mlib::vec<__VECTYPE> v(__end__ - __start__);

      for (size_t i = __start__; i < __end__; i++)
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
        const __VECTYPE temp = _M_container[i];
        _M_container[i] = _M_container[j];
        _M_container[j] = temp;
        j--;
      }
    };

    void reverse(size_t __start__, size_t __end__)
    {
      // reverse from start to end.
      if (this->size() == 0)
        return;

      if (this->size() == 1)
        return;

      const size_t temp_start = __start__;
      const size_t temp_end = __end__;

      if (__start__ > __end__)
      {
        __start__ = temp_end;
        __end__ = temp_start;
      }

      size_t j = __end__;
      for (size_t i = __start__; i < j; i++)
      {
        const __VECTYPE temp = _M_container[i];
        _M_container[i] = _M_container[j];
        _M_container[j] = temp;
        j--;
      }
    };

    // insert non modifiable lvalue or rvalue
    void insert(size_t index, const __VECTYPE &value)
    {
      _Vec_resize_capacity(1);

      size_t shift_index = _M_dynamic_cursor;

      while (shift_index > index)
      {

        // [1, 4, ]
        __VECTYPE *temp = _M_container + shift_index;
        *(temp) = *(temp - 1);
        shift_index--;
      }

      *(_M_container + index) = value;
      _M_dynamic_cursor++;
    };

    size_t size() const { return _M_dynamic_cursor; }
    size_t cursor_byte_size() const noexcept
    {
      return _M_dynamic_cursor * _M_cell_size;
    };
    size_t capacity_byte_size() const noexcept
    {
      return _M_capacity_size * _M_cell_size;
    }
    void push_back(const __VECTYPE &__val__)
    {
      if (_M_dynamic_cursor >= _M_capacity_size)
      {
        // realloc
        _M_container =
            (__VECTYPE *)realloc(_M_container, (++_M_capacity_size) * _M_cell_size);
      }

      *(_M_container + _M_dynamic_cursor) = __val__;
      _M_dynamic_cursor++;
    };

    ~vec()
    {
      clear();
    };
  }; // end class vec
} // namespace mlib