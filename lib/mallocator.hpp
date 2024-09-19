#pragma once
#include "memory.h"

namespace mlib
{

    template <typename _AllocTy>
    struct allocator
    {

        typedef _AllocTy *_ATy_pointer;
        typedef const _AllocTy *_ATy_const_pointer;
        typedef _AllocTy _ATy_value;
        typedef _AllocTy &_ATy_reference;
        typedef const _AllocTy &_ATy_const_reference;

        allocator() noexcept {};
        allocator(const allocator<_AllocTy> &_other_) noexcept {};

        template <typename _OtherAllocTy>
        allocator(const allocator<_OtherAllocTy> &_other_) noexcept;

        _ATy_pointer allocate(size_t _n_)
        {
            _ATy_pointer alloc_block = (_ATy_pointer)malloc(_n_ * sizeof(_ATy_value));
            if (!alloc_block)
            {
                return nullptr;
            };
            return alloc_block;
        };

        _ATy_pointer reallocate(_ATy_pointer _old_, size_t _n_)
        {
            _old_ = (_ATy_pointer)realloc(_old_, _n_ * sizeof(_ATy_value));
            return _old_;
        }

        template <class _T, class... _FwdArgs>
        void construct(_T *_loc_, _FwdArgs &&..._args_)
        {
            ::new (_loc_) _T(std::forward<_FwdArgs>(_args_)...);
        };

        template <class _T>
        void destroy(_T *_loc_)
        {
            _loc_->~_T();
        }

        void deallocate(_ATy_pointer _region_)
        {
            free(_region_);
        };

        _ATy_pointer address(_ATy_reference _ref_) noexcept
        {
            return &_ref_;
        };

        ~allocator() noexcept {};
    };

}
