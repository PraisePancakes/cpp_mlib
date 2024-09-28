#pragma once
#include <type_traits>

namespace mlib
{
    template <typename _Ty, _Ty _v_>
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

    template <typename _Ty>
    struct add_const
    {
        using type = const _Ty;
    };

    template <typename _Ty>
    struct add_const_t
    {
        using type = typename add_const<_Ty>::type;
    };

    template <typename _Ty>
    struct add_volatile
    {
        using type = volatile _Ty;
    };

    template <typename _Ty>
    struct add_volaitle_t
    {
        using type = add_volatile<_Ty>::type;
    };

    template <typename _Ty>
    struct add_cv
    {
        using type = const volatile _Ty;
    };

    template <typename _Ty>
    struct add_cv_t
    {
        using type = add_cv<_Ty>::type;
    };

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    namespace is_class_detail
    {
        template <typename T>
        bool_constant<!std::is_union<T>::value> test(int T::*);

        template <typename T>
        false_type test(...);
    }

    template <typename _Ty>
    struct is_class : decltype(is_class_detail::test<_Ty>(nullptr))
    {
    };

    namespace is_base_detail
    {
        // template helper to check valid conversion
        template <typename _B>
        true_type test_ptr_conv(const volatile _B *);

        // template helper to assert fallback conversion
        template <typename>
        false_type test_ptr_conv(const volatile void *);

        // check valid base conversion
        template <typename _B, typename _D>
        auto test_is_base_of(int) -> decltype(test_ptr_conv<_B>(static_cast<_D *>(nullptr)));

        // check fallback base conversion
        template <typename, typename>
        auto test_is_base_of(...) -> true_type;
    };

    template <typename _B, typename _D>
    struct is_base_of : bool_constant<
                            is_class<_B>::value &&
                            is_class<_D>::value &&decltype(is_base_detail::test_is_base_of<_B, _D>(0))::value>
    {
    };

    template <typename _Ty>
    constexpr bool is_class_v = is_class<_Ty>::value;

    template <typename _Ty>
    struct is_ptr : false_type
    {
    };

    template <typename _Ty>
    struct is_ptr<_Ty *> : true_type
    {
    };

    template <typename _Ty>
    struct is_void : false_type
    {
    };

    template <>
    struct is_void<void> : true_type
    {
    };

    template <typename _Ty>
    constexpr bool is_void_v = is_void<_Ty>::value;

    template <typename _Ty>
    struct remove_const
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_const<const _Ty>
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_volatile
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_volatile<volatile _Ty>
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_cv
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_cv<const volatile _Ty>
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_q
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_q<const _Ty>
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_q<volatile _Ty>
    {
        typedef _Ty type;
    };

    template <typename _Ty>
    struct remove_q<const volatile _Ty>
    {
        typedef _Ty type;    
    };

    

}
