#pragma once
#include "miterator.hpp"
/*
ALG LIST
    -- for_each
*/
#define QSORT_THRESHOLD 20000

namespace mlib
{
    template <typename T>
    constexpr void swap(T &_v1_, T &_v2_) noexcept(std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_move_assignable_v<T> && std::is_move_constructible_v<T>)
    {
        T temp = std::move(_v1_);
        _v1_ = std::move(_v2_);
        _v2_ = std::move(temp);
    };

    namespace internal
    {
        template <typename RAIT>
        std::ptrdiff_t _internal_iterator_diff(RAIT s, RAIT e)
        {
            return e.get() - s.get();
        };

        template <typename RAIT>
        class _internal_quick_sort_functor
        {

        protected:
            typedef mlib::iterator_traits<RAIT> it_traits;
            typedef it_traits::value_type value_type;
            typedef it_traits::reference reference;
            typedef it_traits::size_type size_type;

            RAIT first;
            RAIT last;
            size_type size;

            RAIT _partition(RAIT s, RAIT e)
            {
                RAIT pivot = e;

                RAIT i = s - 1;
                for (RAIT j = s; j < e; j++)
                {
                    if (*j <= *pivot)
                    {
                        i++;
                        mlib::swap(*i, *j);
                    }
                }

                mlib::swap(*(i + 1), *e);

                return i + 1;
            };

            void _quick_sort(RAIT start, RAIT end)
            {
                if (start >= end)
                    return;

                RAIT pivot = _partition(start, end);

                _quick_sort(start, pivot - 1);
                _quick_sort(pivot + 1, end);
            };

        public:
            _internal_quick_sort_functor(RAIT start, RAIT end, size_type size) : first(start), last(end), size(size)
            {
                (*this)();
            };
            void operator()()
            {
                _quick_sort(first, last - 1);
            };

            ~_internal_quick_sort_functor() = default;
        };

    }

    template <typename RAIT>
    void sort(RAIT _first_, RAIT _last_)
    {
        typename iterator_traits<RAIT>::difference_type size = _last_ - _first_;

        if (size <= QSORT_THRESHOLD)
        {
            internal::_internal_quick_sort_functor<RAIT>(_first_, _last_, size);
        };
    }

    template <typename InputIt, typename UFunc>
    UFunc for_each(InputIt it1, InputIt it2, UFunc f)
    {
        for (; it1 != it2; it1++)
        {
            f(*it1);
        }
        return f;
    };

    template <typename BidIt>
    void reverse(BidIt _first_, BidIt _last_)
    {

        while (_first_ != _last_)
        {
            typename mlib::iterator_traits<BidIt>::value_type temp = *_first_;
            *_first_ = *_last_;
            *_last_ = temp;
            ++_first_;
            if (_first_ == _last_)
                return;
            --_last_;
        }
    };

    template <typename BidIt>
    void reverse(BidIt _first_, BidIt _last_, size_t start, size_t end)
    {
        _first_ += start;
        _last_ = _first_ + end;

        reverse(_first_, _last_);
    }

    template <typename InputIt, typename T = typename mlib::iterator_traits<InputIt>::value_type>
    constexpr InputIt find(InputIt _first_, InputIt _last_, const T &val)
    {

        InputIt hold_last_ub = _last_;

        while (_first_ != _last_)
        {
            --_last_;
            if (*_first_ == val)
                return _first_;
            if (*_last_ == val)
                return _last_;
            ++_first_;
        }

        return hold_last_ub;
    };

}
