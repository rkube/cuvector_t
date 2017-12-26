#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <cassert>

#include "datatypes.h"
#include "iterators.h"


template <typename T> class iter2d;

template <typename T>
class vector2d
{
    public:
        using iterator_t = iter2d<T>;
        vector2d(const slab_layout_t<T>);
        vector2d(const vector2d<T>&);
        ~vector2d();
        
        iterator_t begin() const
        {
            return (iterator_t(this -> get_data(), get_sl(), 0, 0, get_transformed()));
        }

        iterator_t end() const
        {
            // If the vector is transformed the end of the range is get_nx() + get_padx().
            // If the vector is not transformed the end of the range is get_nx()
            return(iterator_t(this -> get_data(), get_sl(),
                              get_transformed() ? get_sl().get_nx() + get_sl().get_padx(): get_sl().get_nx() ,
                              get_transformed() ? get_sl().get_my() + get_sl().get_pady(): get_sl().get_my(),
                              get_transformed()));
        }
        
        void operator= (const vector2d<T>& rhs);

        T* get_data() const { return(data); }
        slab_layout_t<T> get_sl() const { return(sl); }
        size_t size() const { return( (get_sl().get_nx() + get_sl().get_padx()) * (get_sl().get_my() + get_sl().get_pady())); }

        bool get_transformed() const { return(transformed); }
        void set_transformed(bool tr) { transformed = tr; }

    private:
        const slab_layout_t<T> sl;
        const size_t nelem;
        bool transformed;
        T* data;
};


template <typename T>
vector2d<T> :: vector2d(const slab_layout_t<T> _sl) : 
    sl(_sl),
    //nelem((sl.get_nx() + sl.get_padx()) * (sl.get_my() + sl.get_pady())),
    nelem(size()),
    transformed(false), 
    data{new T[nelem]}
{
    // Use reference here!
    size_t n{0};
    for(auto& i : (*this))
    {
        i = 2.19;
    }
}


template <typename T>
vector2d<T> :: vector2d(const vector2d<T>& rhs) : 
    sl(rhs.get_sl()),
    nelem(rhs.size()), 
    transformed(rhs.get_transformed()), 
    data{new T[rhs.size()]}
{
    //auto r = rhs.begin();
    //for(auto& i : (*this))
    //{
    //    i = *r++;
    //}
}


template <typename T>
void vector2d<T> :: operator=(const vector2d<T>& rhs)
{
    assert(get_sl() == rhs.get_sl());
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

#endif // MYVECTOR_H
