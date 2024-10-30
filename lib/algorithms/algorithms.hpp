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
    void reverse(BidIt first, BidIt last)
    {
        while (first != last && first != --last)
        {
            typename mlib::iterator_traits<BidIt>::value_type temp = *first;
            *first = *last;
            *last = temp;
            ++first;
        }
    };

    template <typename BidIt>
    void reverse(BidIt first, BidIt last, size_t start, size_t end)
    {
        first += start;
        last = first + end;
        reverse(first, last);
    }

    template <typename InputIt, typename T = typename mlib::iterator_traits<InputIt>::value_type>
    constexpr InputIt find(InputIt first, InputIt last, const T &val)
    {

        InputIt hold_last_ub = last;

        while (first != last)
        {
            --last;
            if (*first == val)
                return first;
            if (*last == val)
                return last;
            ++first;
        }

        return hold_last_ub;
    };

}
