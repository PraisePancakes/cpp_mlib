#pragma once
#include "mvector.hpp"
#include <string>

namespace mlib
{

    class string
    {
    private:
        struct _underlying_const_alloc
        {

            const char *internal_str;
            size_t internal_byte_size;

            _underlying_const_alloc()
            {

                this->internal_byte_size = 0;
                this->internal_str = nullptr;
            }

            _underlying_const_alloc(const char *__str__)
            {
                internal_str = __str__;
                internal_byte_size = sizeof(char) * _M_get_size(__str__);
            }

            void operator=(const char *__other__)
            {
                this->internal_str = __other__;
                this->internal_byte_size = sizeof(char) * _M_get_size(__other__);
            }

        private:
            const size_t _M_get_size(const char *__str__) const
            {
                size_t i = 0;

                while (__str__[i] != '\0')
                {
                    i++;
                }

                return i + 1;
            }
        };

        _underlying_const_alloc *_M_internal_str;
        size_t _M_s_size = 0;
        char *_M_modifiable_str;

        void _copy_to_modifiable(const _underlying_const_alloc &__str__)
        {

            const size_t str_size = sizeof(char) * __str__.internal_byte_size;

            _M_modifiable_str = (char *)malloc(str_size);
            for (size_t i = 0; i < str_size; i++)
            {
                _M_modifiable_str[i] = __str__.internal_str[i];
            }

            _M_s_size = str_size;
        }

        void _perform_deep_copy(const char *__str__)
        {
            _M_internal_str = new _underlying_const_alloc(__str__);
            _copy_to_modifiable(*_M_internal_str);
        };

        void _perform_deep_copy(const mlib::string &__other__)
        {
            _M_internal_str = new _underlying_const_alloc(__other__.data());
            _copy_to_modifiable(*_M_internal_str);
        };

    public:
        class iterator
        {
            char *_Iterator_ptr;

        public:
            iterator() : _Iterator_ptr(nullptr) {}
            iterator(char *__iter_loc__) : _Iterator_ptr(__iter_loc__) {}

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

            char &operator*() const
            {
                return *(_Iterator_ptr);
            };
            ~iterator() {};
        };

        string(const char *__str__)
        {
            _perform_deep_copy(__str__);
        };

        string()
        {
            _perform_deep_copy("");
        };

        string(const mlib::string &__other__)
        {
            _perform_deep_copy(__other__);
        }

        string(mlib::string &__other__)
        {
            _perform_deep_copy(__other__);
        }

        string(mlib::string &&__other__)
        {
            _perform_deep_copy(__other__);
        }
        char *data() const
        {
            return _M_modifiable_str;
        }

        char &operator[](size_t __index__)
        {
            return _M_modifiable_str[__index__];
        };

        char *begin() const noexcept
        {
            return this->_M_modifiable_str;
        }
        char *end() const noexcept
        {
            return this->_M_modifiable_str + this->length();
        }

        mlib::string &append(const mlib::string &__other__)
        {
            // get __str__ size
            this->resize(__other__.length());
            // helloworld len 10  : '\0' at 11
            const size_t next_open_cell = this->length() - __other__.length();
            size_t other_cursor = 0;
            for (size_t i = next_open_cell; i < this->length(); i++)
            {
                this->_M_modifiable_str[i] = __other__.data()[other_cursor];
                other_cursor++;
            }

            _M_modifiable_str[this->length()] = '\0';

            // place null terminator at this->length() + 1;
            return *this;
        }

        mlib::string &operator+=(const mlib::string &__other__)
        {
            return this->append(__other__);
        }

        friend std::ostream &operator<<(std::ostream &__os__, const mlib::string &__str__)
        {
            return __os__ << __str__.data();
        }

        void resize(const size_t __sz_off__)
        {
            this->_M_modifiable_str = (char *)realloc(_M_modifiable_str, this->length() + __sz_off__);
            this->_M_s_size = this->length() + __sz_off__ + 1;
        };

        size_t length() const
        {
            return this->_M_s_size - 1;
        };

        size_t size() const
        {
            return this->_M_s_size - 1;
        }

        ~string()
        {

            delete _M_internal_str;
            _M_internal_str = nullptr;

            free(_M_modifiable_str);
            _M_modifiable_str = nullptr;
        };
    };

};