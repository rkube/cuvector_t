#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cassert>
#include <vector>

#include "datatypes.h"
#include "bounds.h"
#include "allocators.h"

/*
namespace detail
{
    template <typename T>
    inline void impl_set_data_tlev_ptr(T* data, T** data_tlev_ptr, const size_t tlevs, const bounds_t& bounds, allocator_host<T>)
    {
        for(size_t t = 0; t < tlevs; t++)
        {
            data_tlev_ptr[t] = data + t * bounds.get_nelem();
        }
    }


    template <typename T>
    inline void impl_advance(T** tlev_ptr, const size_t tlevs, allocator_host<T>)
    {
        T* tmp{tlev_ptr[tlevs - 1]};
        for(size_t t = tlevs - 1; t > 0; t--)
        {
            tlev_ptr[t] = tlev_ptr[t - 1];
        }
        tlev_ptr[0] = tmp;
        tmp = nullptr;
    }
};
*/

template <typename T, template <typename> class allocator>
class vector2d
{
    public:
        using allocator_type = typename my_allocator_traits<T, allocator> :: allocator_type;
        using deleter_type = typename my_allocator_traits<T, allocator> :: deleter_type;
        using ptr_type = std::unique_ptr<T, deleter_type>;
        // T** pointers
        //using p_allocator_type = typename my_allocator_traits<T*, allocator> :: allocator_type;
        //using p_deleter_type = typename my_allocator_traits<T*, allocator> :: deleter_type;
        //using pptr_type = std::unique_ptr<T*, p_deleter_type>;

        vector2d(const bounds_t);
                
        void operator= (const vector2d<T, allocator>& rhs);

        //inline bool is_transformed(const size_t tidx) const {return(transformed[tidx]);};
        //inline bool set_transformed(const size_t tidx, const bool val) 
        //{
        //    return(transformed[tidx] = val);
        //};

        T* get_data() const { return(data.get()); }
        bounds_t get_bounds() const { return(bounds); }
        size_t size() const { return(bounds().size()); }
        //size_t get_tlevs() const {return(tlevs);}

        //inline T* get_tlev_ptr(const size_t tidx) const
        //{
        //    assert(tidx < get_tlevs());
        //    return(detail :: impl_get_data_tlev_ptr(get_tlev_ptr(), tidx, get_tlevs(), allocator_type{}));   
        //};

    private:
        allocator_type my_alloc; 
        //p_allocator_type my_palloc;
        const bounds_t bounds;
        //const size_t tlevs;
        //std::vector<bool> transformed;
        ptr_type data;
        //pptr_type tlev_ptr;
};


template <typename T, template <typename> class allocator>
vector2d<T, allocator> :: vector2d(const bounds_t b_) :
    bounds(b_),
    data(my_alloc.allocate(b_.size()))
{
}



template <typename T, template<typename> class allocator>
void vector2d<T, allocator> :: operator=(const vector2d<T, allocator>& rhs)
{
    assert(get_bounds() == rhs.get_bounds());
}


#endif // VECTOR2D_H
