#pragma once
#include "memory.h"

namespace mlib
{

    template <typename _AllocTy>
    struct allocator_traits
    {

        typedef _AllocTy *pointer;
        typedef const _AllocTy *const_pointer;
        typedef _AllocTy value_type;
        typedef _AllocTy &reference;
        typedef const _AllocTy &const_reference;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        static pointer allocate(size_t _n_)
        {
            pointer alloc_block = (pointer)malloc(_n_ * sizeof(value_type));
            if (!alloc_block)
            {
                std::cout << "null";
                return nullptr;
            };
            return alloc_block;
        };

        static void allocate_at(pointer &_loc_, difference_type _n_)
        {
            _loc_ = (pointer)malloc(_n_ * sizeof(value_type));
            if (!_loc_)
            {
                _loc_ = nullptr;
            }
        }

        static void reallocate_at(pointer &_loc_, difference_type _n_)
        {
            _loc_ = (pointer)realloc(_loc_, _n_ * sizeof(value_type));
            if (!_loc_)
            {
                _loc_ = nullptr;
            }
        }

        static pointer reallocate(pointer _old_, difference_type _n_)
        {
            _old_ = (pointer)realloc(_old_, _n_ * sizeof(value_type));
            return _old_;
        }

        template <class... _FwdArgs>
        static void construct(pointer _loc_, _FwdArgs &&..._args_)
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

    template <typename _AllocTy>
    struct allocator
    {

        typedef typename allocator_traits<_AllocTy>::pointer pointer;
        typedef typename allocator_traits<_AllocTy>::const_pointer const_pointer;
        typedef typename allocator_traits<_AllocTy>::value_type value_type;
        typedef typename allocator_traits<_AllocTy>::reference reference;
        typedef typename allocator_traits<_AllocTy>::const_reference const_reference;

        allocator() noexcept {};
        allocator(const allocator<_AllocTy> &_other_) noexcept {};

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

        void deallocate(pointer _region_)
        {
            free(_region_);
        };

        pointer address(reference _ref_) noexcept
        {
            return &_ref_;
        };

        ~allocator() noexcept {};
    };

}
