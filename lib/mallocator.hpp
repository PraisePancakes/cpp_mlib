#pragma once
#include "memory.h"

namespace mlib
{

    template <typename Alloc>
    struct allocator_traits
    {

        typedef Alloc allocator_type;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;
        typedef typename Alloc::value_type value_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::size_type size_type;
        typedef typename Alloc::difference_type difference_type;

    private:
        static constexpr bool has_allocate =
            requires(Alloc &a, size_type n) {
                { a.allocate(n) } -> std::same_as<typename Alloc::pointer>;
            };

        static constexpr bool has_reallocate =
            requires(Alloc &a, pointer old, size_type n) {
                { a.reallocate(old, n) } -> std::same_as<typename Alloc::pointer>;
            };

    public:
        static pointer
        allocate(Alloc &alloc, size_type n)
        {
            if constexpr (has_allocate)
            {
                return alloc.allocate(n);
            }
            else
            {
                return (pointer)malloc(n * sizeof(value_type));
            }
        }

        static pointer reallocate(Alloc &alloc, pointer _old_, difference_type _n_)
        {
            if constexpr (has_reallocate)
            {

                return alloc.reallocate(_old_, _n_);
            }
            else
            {
                pointer new_loc = (pointer)realloc(_old_, _n_ * sizeof(value_type));
                if (!new_loc)
                {
                    std::cout << "null";
                    return nullptr;
                }
                return new_loc;
            }
        }

        template <class... _FwdArgs>
        static void construct(Alloc &alloc, pointer _loc_, _FwdArgs &&..._args_)
        {
            ::new (_loc_) value_type(std::forward<_FwdArgs>(_args_)...);
        };

        static void destroy(pointer _loc_)
        {
            _loc_->~value_type();
        }

        static void deallocate(pointer _region_)
        {
            free(_region_);
        };

        static pointer address(reference _ref_) noexcept
        {
            return &_ref_;
        };
    };

    template <typename T>
    struct allocator
    {

        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        allocator() noexcept {};
        allocator(const allocator<T> &_other_) noexcept {};

        template <typename _OtherAllocTy>
        allocator(const allocator<_OtherAllocTy> &_other_) noexcept {};

        pointer allocate(size_t _n_)
        {
            pointer alloc_block = (pointer)malloc(_n_ * sizeof(value_type));
            if (!alloc_block)
            {
                return nullptr;
            };
            return alloc_block;
        };

        pointer reallocate(pointer _old_, size_t _n_)
        {
            pointer new_block = (pointer)realloc(_old_, _n_ * sizeof(value_type));
            if (new_block == nullptr)
            {

                return _old_;
            }

            _old_ = new_block;
            return _old_;
        }

        template <class... _FwdArgs>
        void construct(pointer _loc_, _FwdArgs &&..._args_)
        {
            if (_loc_ == nullptr)
            {

                return;
            }
            ::new (_loc_) value_type(std::forward<_FwdArgs>(_args_)...);
        };

        void destroy(pointer _loc_)
        {
            _loc_->~value_type();
        }

        void deallocate(pointer _region_, size_type _n_)
        {
            free(_region_);
        };

        pointer address(reference _ref_) noexcept
        {
            return &_ref_;
        };

        friend bool operator==(const allocator &, const allocator &) { return true; }
        friend bool operator!=(const allocator &, const allocator &) { return false; }

        ~allocator() noexcept {};
    };

    template <>
    class allocator<void>
    {
        typedef void value_type;
        typedef void *void_pointer;
        typedef const void *const_void_pointer;
    };

}
