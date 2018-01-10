#ifndef UTILITY_H
#define UTILITY_H

#include "view.h"
#include <iomanip>

namespace utility
{
    template <typename T, typename F>
    void apply(strided_view<T>& view, F myfunc)
    {
        for(auto& it : view.get_bounds())
        {
            view[it] = myfunc(view[it], it, view.get_geom());
        }
    }

    template <typename T, typename F>
    void elementwise(strided_view<T>& lhs, strided_view<T>& rhs, F myfunc)
    {
        assert(lhs.get_bounds() == rhs.get_bounds());
        {
            for(auto& it : lhs.get_bounds())
            {
                lhs[it] = myfunc(lhs[it], rhs[it], it, lhs.get_geom());
            }
        }
    }

    template<typename T>
    void print(strided_view<T>& view)
    {
        for(auto it : view.get_bounds())
        {
            std::cout << std::setw(10) << view[it] << "\t";
            if(it[1] == view.get_bounds().get_my() - 1)
                std::cout << std::endl;
        }
    }

}


#endif //UTILITY_H