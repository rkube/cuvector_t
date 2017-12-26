#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cassert>

#include "datatypes.h"
#include "bounds.h"


template <typename T>
class vector2d
{
    public:
        vector2d(const bounds_t);
        ~vector2d();
                
        void operator= (const vector2d<T>& rhs);

        T* get_data() const { return(data); }
        bounds_t get_bounds() const { return(bounds); }
        size_t size() const { return( return(get_bounds().size()); }

    private:
        const bounds_t bounds;
        T* data;
};


template <typename T>
vector2d<T> :: vector2d(const bounds_t b_) :
    bounds(b_),
    data{new T[nelem]}
{
}



template <typename T>
void vector2d<T> :: operator=(const vector2d<T>& rhs)
{
    assert(bounds() == rhs.bounds());
/*
    set_transformed(rhs.get_transformed());
    auto r = rhs.begin();
    for(auto& i : (*this))
    {
        i = *r++;
    }
*/
}

template <typename T>
vector2d<T> :: ~vector2d()
{
    delete [] data;
}

#endif // VECTOR2D_H
