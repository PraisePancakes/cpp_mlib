#pragma once
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
        size_t _internal_iterator_diff(RAIT s, RAIT e)
        {
            return e.get() - s.get();
        };

        template <typename RAIT>
        class _internal_quick_sort_helper
        {

        protected:
            typedef mlib::iterator_traits<RAIT> it_traits;
            typedef it_traits::value_type value_type;
            typedef it_traits::reference reference;

            RAIT _hold_container_start;
            size_t first;
            size_t last;

        private:
            size_t _partition(size_t start, size_t end)
            {
                reference pivot = _hold_container_start[end];
                size_t i = start;
                for (size_t j = start; j < end; j++)
                {
                    if (_hold_container_start[j] < pivot)
                    {

                        mlib::swap(_hold_container_start[i], _hold_container_start[j]);
                        i++;
                    }
                };

                mlib::swap(_hold_container_start[i], _hold_container_start[end]);

                return i;
            };

            void _quick_sort(size_t start, size_t end)
            {
                if (start >= end)
                    return;

                size_t pivot = _partition(start, end);

                if (pivot > 0)
                    _quick_sort(start, pivot - 1);
                _quick_sort(pivot + 1, end);
            };

        public:
            _internal_quick_sort_helper(RAIT _first_, RAIT _last_) : _hold_container_start(_first_), first(0), last(_internal_iterator_diff(_first_, _last_) - 1) {

                                                                     };

            void sort()
            {

                _quick_sort(first, last);
            };

            ~_internal_quick_sort_helper() = default;
        };

    }

    template <typename RAIT>
    void sort(RAIT _first_, RAIT _last_)
    {
        if (mlib::internal::_internal_iterator_diff(_first_, _last_) <= QSORT_THRESHOLD)
        {
            mlib::internal::_internal_quick_sort_helper<RAIT> h(_first_, _last_);
            h.sort();
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
