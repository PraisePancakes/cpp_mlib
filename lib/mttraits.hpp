#pragma once
#include <type_traits>

namespace mlib
{
    template <class _Ty, _Ty _v_>
    struct integral_constant
    {
        static constexpr _Ty value = _v_;
        using value_type = _Ty;
        using type = integral_constant;

        constexpr operator value_type() const noexcept
        {
            return value;
        };

        [[nodiscard]] constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    template <bool _V>
    using bool_constant = integral_constant<bool, _V>;

    template <class _Ty>
    struct add_const
    {
        using type = const _Ty;
    };

    template <class _Ty>
    struct add_const_t
    {
        using type = typename add_const<_Ty>::type;
    };

    template <class _Ty>
    struct add_volatile
    {
        using type = volatile _Ty;
    };

    template <class _Ty>
    struct add_volaitle_t
    {
        using type = add_volatile<_Ty>::type;
    };

    template <class _Ty>
    struct add_cv
    {
        using type = const volatile _Ty;
    };

    template <class _Ty>
    struct add_cv_t
    {
        using type = add_cv<_Ty>::type;
    };

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    namespace detail
    {
        template <class T>
        integral_constant<bool, !std::is_union<T>::value> test(int T::*);

        template <class T>
        false_type test(...);
    }

    template <class _Ty>
    struct is_class : decltype(detail::test<_Ty>(nullptr))
    {
    };

    template <class _Ty>
    constexpr bool is_class_v = is_class<_Ty>::value;

    template <class _Ty>
    struct is_ptr : false_type
    {
    };

    template <class _Ty>
    struct is_ptr<_Ty *> : true_type
    {
    };

}
