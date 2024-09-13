#pragma once
#include "mvector.hpp"
#include <string>

namespace mlib
{
    class string
    {
        char *_M_str;
        size_t _M_s_size = 0;

        size_t _get_size(char *__str__) const
        {
            size_t i = 0;
            while (*(__str__ + i) != '\0')
            {
                i++;
            }
            return i;
        }

    public:
        string(char *__str__)
        {
            this->_M_str = __str__;
            _M_s_size = _get_size(__str__);
        };

        const char *data() const
        {
            return _M_str;
        }

        size_t length() const
        {
            return _M_s_size;
        }

        size_t size() const
        {
            return _M_s_size;
        }

        char &operator[](size_t __index__) const
        {
            return _M_str[__index__];
        };

        friend std::ostream &
        operator<<(std::ostream &os, const mlib::string &str)
        {
            os << str.data();
            return os;
        }
    };

};