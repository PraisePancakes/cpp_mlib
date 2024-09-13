#pragma once
#include "mvector.hpp"
#include <string>

namespace mlib
{

    class string
    {

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
                std::cout << __str__;
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
            std::cout << __str__.internal_str << str_size;
            _M_modifiable_str = (char *)malloc(str_size);
            for (size_t i = 0; i < str_size; i++)
            {
                _M_modifiable_str[i] = __str__.internal_str[i];
            }

            _M_s_size = str_size;
        }

    public:
        string(const char *__str__)
        {

            _M_internal_str = new _underlying_const_alloc(__str__);
            _copy_to_modifiable(*_M_internal_str);
        };

        char *data() const
        {
            return _M_modifiable_str;
        }

        char &operator[](size_t __index__)
        {
            return _M_modifiable_str[__index__];
        };

        friend std::ostream &operator<<(std::ostream &__os__, const mlib::string &__str__)
        {
            return __os__ << __str__.data();
        }
    };

};