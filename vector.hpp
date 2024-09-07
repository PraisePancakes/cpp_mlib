#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
#include <exception>
#include <initializer_list>



#define __INITIAL_VECTOR_CAPACITY__ 1


  inline ptrdiff_t address_diff_in_bytes(const void *addr1, const void *addr2) {
    return std::abs(static_cast<const char *>(addr1) -
                    static_cast<const char *>(addr2));
  }

namespace mlib {
template <typename T> class vec {
  private:
    T *_Vec_container;
    size_t _Vec_capacity_size;
    size_t _Vec_dynamic_cursor;
    size_t _Vec_cell_size;

    void _Vec_init_container(size_t __size__) {
      this->_Vec_capacity_size = __size__;
      _Vec_dynamic_cursor = 0;
      this->_Vec_cell_size = sizeof(T);
      this->_Vec_container = (T *)malloc(__size__ * _Vec_cell_size);
    }

    void _Vec_deep_copy(const vec &__other__) {
      _Vec_init_container(__other__.size());
      size_t safe_iterator = 0;
      for (size_t i = 0; i < __other__.size(); i++) {
        _Vec_container[safe_iterator] = __other__[i];
        safe_iterator++;
        _Vec_dynamic_cursor++;
      }
    };

    void _Vec_resize_capacity(const size_t __capacity_size_offset__) {

      const size_t new_size =
          (_Vec_capacity_size + __capacity_size_offset__) * _Vec_cell_size;
      this->_Vec_container = (T *)realloc(_Vec_container, new_size);
      _Vec_capacity_size += __capacity_size_offset__;
    };

   
  public:

    class iterator {
      T* _Iterator_ptr;
    
      public:
      iterator() : _Iterator_ptr(nullptr) {}
      iterator(T* __iter_loc__) : _Iterator_ptr(__iter_loc__) {}

      bool operator!=(const iterator& other) {
        
        return _Iterator_ptr != other._Iterator_ptr;
      };

      iterator& operator++(){
         
        ++_Iterator_ptr;
        return *this;
      }
      iterator& operator--(){

        --_Iterator_ptr;
        return *this;
      }
      
      T& operator*() const {
        return *(_Iterator_ptr);
      };
      ~iterator() {};
    };

    class reverse_iterator {
      T* _Iterator_ptr;

      public:
      reverse_iterator() : _Iterator_ptr(nullptr) {};
      reverse_iterator(T* __iter_loc__) : _Iterator_ptr(__iter_loc__) {};
      reverse_iterator(const reverse_iterator& other) = delete;
      reverse_iterator(reverse_iterator&& other) = delete;
      

      bool operator!=(const reverse_iterator& other) {

        return _Iterator_ptr != other._Iterator_ptr;
      };

      reverse_iterator& operator++() {
          --_Iterator_ptr;  
          return *this;
      }

       T& operator*() {
          return *(_Iterator_ptr);
      };

       reverse_iterator& operator--() {

        _Iterator_ptr++;
        return *this;
      }
    
    };

    class const_reverse_iterator {
      T* _Iterator_ptr;
      
      public:
      const_reverse_iterator() : _Iterator_ptr(nullptr) {};
      const_reverse_iterator(T* __iter_loc__) : _Iterator_ptr(__iter_loc__) {};
      const_reverse_iterator(const const_reverse_iterator& other) = delete;
      const_reverse_iterator(const_reverse_iterator&& other) = delete;
    
      bool operator!=(const const_reverse_iterator& other) {
      
        return _Iterator_ptr != other._Iterator_ptr;
      };
      
      const const_reverse_iterator& operator++() {
          --_Iterator_ptr;  
          return *this;
      }
      
      const T& operator*() {
          return *(_Iterator_ptr);
      };
      
      const const_reverse_iterator& operator--() {
      
        _Iterator_ptr++;
        return *this;
      };
      ~const_reverse_iterator() {};
    };


    class const_iterator {
      T* _Iterator_ptr;
    
      public:
      const_iterator() : _Iterator_ptr(nullptr) {}
      const_iterator(T* __iter_loc__) : _Iterator_ptr(__iter_loc__) {}
      const_iterator(const const_iterator& other) = delete;
      const_iterator(const_iterator&& other) = delete;
      

      bool operator!=(const const_iterator& other) {
    
        return _Iterator_ptr != other._Iterator_ptr;
      };
    
      const const_iterator& operator++() {
    
        ++_Iterator_ptr;
        return *this;
      }
    
      const T& operator*() const {
        return *(_Iterator_ptr);
      };
      ~const_iterator() {};
    };

    
    vec(const size_t &__container_size__) {
      _Vec_init_container(__container_size__);
    };

    iterator mbegin() const {
        return iterator(_Vec_container);
    }

    iterator mend() const {
      return iterator(_Vec_container + _Vec_dynamic_cursor);
    }

    const_iterator mcbegin() const {
      return const_iterator(_Vec_container);
    }

    const_iterator mcend() const {
        return const_iterator(_Vec_container + _Vec_dynamic_cursor);
    }

  
    reverse_iterator mrbegin() const {
      return reverse_iterator(_Vec_container + (_Vec_dynamic_cursor - 1));
    }

    reverse_iterator mrend() const {
      return reverse_iterator(_Vec_container - 1);
    }

    const_reverse_iterator mcrbegin() const {
      return const_reverse_iterator(_Vec_container + (_Vec_dynamic_cursor - 1));
    }

    const_reverse_iterator mcrend() const {
      return const_reverse_iterator(_Vec_container - 1);
    }
 
    vec() { _Vec_init_container(__INITIAL_VECTOR_CAPACITY__); };
   
    vec(std::initializer_list<T> __elems__) {
      
      _Vec_init_container(__elems__.size());

      size_t index = 0;
      for(auto it = __elems__.begin(); it != __elems__.end(); it++) {
          *(_Vec_container + index) = *it;
          index++;
          _Vec_dynamic_cursor++;
      }
      
    };
    vec(const vec &__other__) { _Vec_deep_copy(__other__); };
    vec& operator=(const vec &__other__) {
        if (this != &__other__) {
            free(_Vec_container);
            _Vec_deep_copy(__other__);
        }
        return *this;
    }
    // api
    T &operator[](const size_t __ptr_index__) const {
      return *(_Vec_container + __ptr_index__);
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

    T &at(int __ptr_index__) const {
      //bound check
        std::cout << __ptr_index__ << std::endl;
        std::cout << _Vec_dynamic_cursor << std::endl;
      if(__ptr_index__ < 0) {

         throw std::out_of_range("\nIndex succeeded left bound with underflowed range index, out of range");
      }
        if(__ptr_index__ >= _Vec_dynamic_cursor) {
            throw std::out_of_range("\nIndex succeeded right bound with overflowed range index, out of range ");
        } 
       

        return *(_Vec_container + __ptr_index__);

    };

    bool empty() const {
      return _Vec_dynamic_cursor == 0;
    }
    
    void pop_back() noexcept {
      --_Vec_dynamic_cursor;
      (_Vec_container + _Vec_dynamic_cursor)->~T();
    };

    void for_each(std::function<void(T)> __functor__) noexcept {
      for (size_t i = 0; i < _Vec_capacity_size; i++) {
        __functor__(*(_Vec_container + i));
      }
    };

    // insert non modifiable lvalue or rvalue
    void insert(size_t index, const T &value) {
      _Vec_resize_capacity(1);

      size_t shift_index = _Vec_dynamic_cursor;

      while (shift_index > index) {

        // [1, 4, ]
        T *temp = _Vec_container + shift_index;
        *(temp) = *(temp - 1);
        shift_index--;
      }

      *(_Vec_container + index) = value;
      _Vec_dynamic_cursor++;
    };

    size_t size() const { return _Vec_dynamic_cursor; }
    size_t cursor_byte_size() const noexcept {
      return _Vec_dynamic_cursor * _Vec_cell_size;
    };
    size_t capacity_byte_size() const noexcept {
      return _Vec_capacity_size * _Vec_cell_size;
    }
    void push_back(const T &__val__) {
      if (_Vec_dynamic_cursor >= _Vec_capacity_size) {
        // realloc
        _Vec_container =
            (T *)realloc(_Vec_container, (++_Vec_capacity_size) * _Vec_cell_size);
      }

      *(_Vec_container + _Vec_dynamic_cursor) = __val__;
      _Vec_dynamic_cursor++;
    };

    ~vec(){};
  }; //end class vec
} // namespace mlib

