#pragma once
/*
ALG LIST
    -- for_each
*/

namespace mlib
{
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

    template <typename T>
    constexpr void swap(T &_v1_, T &_v2_) noexcept(std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_move_assignable_v<T> && std::is_move_constructible_v<T>)
    {
        T temp = std::move(_v1_);
        _v1_ = std::move(_v2_);
        _v2_ = std::move(temp);
    };

}
