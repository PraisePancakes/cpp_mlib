#pragma once
#include <type_traits>

namespace mlib
{
    template <typename T, T _v_>
    struct integral_constant
    {
        static constexpr T value = _v_;
        using value_type = T;
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
    using bool_const = integral_constant<bool, _V>;

    using true_type = bool_const<true>;
    using false_type = bool_const<false>;

    template <class...>
    using void_t = void;

    template <typename T>
    struct is_ptr : false_type
    {
    };

    template <typename T>
    struct is_ptr<T *> : true_type
    {
    };

    template <typename T>
    struct is_enum : bool_const<std::is_enum_v<T>>
    {
    };

    template <typename T>
    struct is_integral : false_type
    {
    };

    template <>
    struct is_integral<int> : true_type
    {
    };

    template <>
    struct is_integral<unsigned int> : true_type
    {
    };

    template <>
    struct is_integral<float> : true_type
    {
    };

    template <>
    struct is_integral<double> : true_type
    {
    };

    template <>
    struct is_integral<bool> : true_type
    {
    };

    template <>
    struct is_integral<char> : true_type
    {
    };

    template <>
    struct is_integral<unsigned char> : true_type
    {
    };

    template <>
    struct is_integral<signed char> : true_type
    {
    };

    template <>
    struct is_integral<wchar_t> : true_type
    {
    };

    template <>
    struct is_integral<char16_t> : true_type
    {
    };

    template <>
    struct is_integral<short int> : true_type
    {
    };

    template <>
    struct is_integral<unsigned short int> : true_type
    {
    };

    template <>
    struct is_integral<long int> : true_type
    {
    };

    template <>
    struct is_integral<unsigned long int> : true_type
    {
    };

    template <>
    struct is_integral<long long> : true_type
    {
    };
    template <>
    struct is_integral<unsigned long long> : true_type
    {
    };

    template <typename T>
    struct is_floating_point : false_type
    {
    };

    template <>
    struct is_floating_point<float> : true_type
    {
    };

    template <>
    struct is_floating_point<double> : true_type
    {
    };

    template <typename T>
    struct remove_const
    {
        typedef T type;
    };

    template <typename T>
    struct remove_const<const T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile
    {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile<volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv<const volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_q
    {
        typedef T type;
    };

    template <typename T>
    struct remove_q<const T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_q<volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_q<const volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct is_arithmetic : bool_const<is_integral<T>::value || is_floating_point<T>::value>
    {
    };

    template <typename T>
    struct is_scalar : bool_const<is_arithmetic<T>::value || is_ptr<T>::value || is_enum<T>::value>
    {
    };

    template <typename T>
    struct is_member_ptr_helper : false_type
    {
    };

    template <typename T, typename U>
    struct is_member_ptr_helper<T U::*> : true_type
    {
    };

    template <typename T>
    struct is_member_ptr : is_member_ptr_helper<typename remove_cv<T>::type>
    {
    };

    template <typename T>
    concept integral = is_integral<T>::value;

    template <typename T>
    struct is_reference : false_type
    {
    };

    template <typename T>
    struct is_reference<T &> : true_type
    {
    };

    template <typename T>
    struct is_reference<T &&> : true_type
    {
    };

    template <typename T>
    struct is_const : false_type
    {
    };

    template <typename T>
    struct is_const<const T> : true_type
    {
    };

    template <typename T>
    struct is_function : bool_const<
                             !is_const<const T>::value &&
                             !is_reference<T>::value>
    {
    };

    template <typename T>
    struct is_iterator : bool_const<
                             mlib::is_ptr<T>::value>
    {
    };

    template <typename T, typename U>
    struct is_same : false_type
    {
    };

    template <typename T>
    struct is_same<T, T> : true_type
    {
    };

    template <typename T>
    struct is_union : bool_const<std::is_union_v<T>>
    {
    };

    template <bool B, typename t = void>
    struct enable_if
    {
    };

    template <typename T>
    struct enable_if<true, T>
    {
        typedef T type;
    };

    template <typename T>
    struct add_const
    {
        using type = const T;
    };

    template <typename T>
    struct add_const_t
    {
        using type = typename add_const<T>::type;
    };

    template <typename T>
    struct add_volatile
    {
        using type = volatile T;
    };

    template <typename T>
    struct add_volaitle_t
    {
        using type = add_volatile<T>::type;
    };

    template <typename T>
    struct add_cv
    {
        using type = const volatile T;
    };

    template <typename T>
    struct add_cv_t
    {
        using type = add_cv<T>::type;
    };

    namespace _is_class_detail
    {
        template <typename T>
        bool_const<!std::is_union<T>::value> test(int T::*);

        template <typename T>
        false_type test(...);
    }

    template <typename T>
    struct is_class : decltype(_is_class_detail::test<T>(nullptr))
    {
    };

    namespace _is_base_detail
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
    struct is_base_of : bool_const<
                            is_class<_B>::value &&
                            is_class<_D>::value &&decltype(_is_base_detail::test_is_base_of<_B, _D>(0))::value>
    {
    };

    template <typename T>
    constexpr bool is_class_v = is_class<T>::value;

    template <typename T>
    struct is_void : false_type
    {
    };

    template <>
    struct is_void<void> : true_type
    {
    };

    template <typename T>
    constexpr bool is_void_v = is_void<T>::value;

}
