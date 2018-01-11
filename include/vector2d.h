#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cassert>
#include <vector>

#include "datatypes.h"
#include "bounds.h"
#include "allocators.h"


template <typename T, template <typename> class allocator>
class vector2d
{
    public:
        using allocator_type = typename my_allocator_traits<T, allocator> :: allocator_type;
        using deleter_type = typename my_allocator_traits<T, allocator> :: deleter_type;
        using ptr_type = std::unique_ptr<T, deleter_type>;

        vector2d(const bounds_t b_) : bounds(b_), data(my_alloc.allocate(b_.size())) {};
                
        //void operator= (const vector2d<T, allocator>& rhs);

        T* get_data() const { return(data.get()); }
        bounds_t get_bounds() const { return(bounds); }
        size_t size() const { return(bounds().size()); }

    private:
        allocator_type my_alloc; 
        const bounds_t bounds;
        ptr_type data;
};

#endif // VECTOR2D_H
