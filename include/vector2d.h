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
        size_t size() const { return(bounds().size()); }

    private:
        const bounds_t bounds;
        T* data;
};


template <typename T>
vector2d<T> :: vector2d(const bounds_t b_) :
    bounds(b_),
    data{new T[b_.size()]}
{
    for(size_t n = 0; n < bounds.size(); n++)
    {
        data[n] = T(0);
    }
}



template <typename T>
void vector2d<T> :: operator=(const vector2d<T>& rhs)
{
    assert(get_bounds() == rhs.get_bounds());
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
