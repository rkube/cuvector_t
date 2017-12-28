#ifndef UTILITY_H
#define UTILITY_H


#include "vector2d.h"

namespace vector
{
    // Add lhs and rhs elementwise over 
    template <typename T>
    void add(vector2d<T>& lhs, const vector2d<T>& rhs, const bounds& b, const offset_t stride)
    {
        std::cout << "vecto2d :: add" << std::endl;
    }

}


#endif UTILITY_H