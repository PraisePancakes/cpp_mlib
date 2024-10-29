#pragma once
/*
ALG LIST
    -- for_each
*/

namespace mlib
{
    template <typename InIt, typename UFunc>
    UFunc for_each(InIt it1, InIt it2, UFunc f)
    {
        for (; it1 != it2; it1++)
        {
            f(*it1);
        }
        return f;
    };

}
