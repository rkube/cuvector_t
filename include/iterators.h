#ifndef ITERATORS_H
#define ITERATORS_H

#include "datatypes.h"


template <typename T> 
class iter2d
{
    public:
        iter2d(T* _ref, const slab_layout_t<T> _sl, const size_t _offset_x, const size_t _offset_y, bool _trans) : 
               data_ref(_ref), sl(_sl), offset_x(_offset_x), offset_y(_offset_y), transformed(_trans) 
        {
            //std::cout << "v_iter(T*, const size_t, const size_t" << std::endl;
            //std::cout << "    data at " << data_ref << ", nelem = " << nelem << ", offset = " << offset << std::endl;
        }
        iter2d(T* _ref, const slab_layout_t<T> _sl, bool _trans) : data_ref(_ref), sl(_sl), offset_x(0), offset_y(0), transformed(_trans)
        {
            //std::cout << "v_iter(T*, const size_t nelem" << std::endl;
            //std::cout << "      data at " << data_ref << ", nelem = " << nelem << std::endl;
        }
    
        size_t get_offset_x() const {return(offset_x);}
        size_t get_offset_y() const {return(offset_y);}

        const slab_layout_t<T> get_sl() const { return(sl); }

        size_t get_max_x() const {return(transformed ? get_sl().get_nx() + get_sl().get_padx() : get_sl().get_nx()); }
        size_t get_max_y() const {return(transformed ? get_sl().get_my() + get_sl().get_pady() : get_sl().get_my()); }


        iter2d<T>& operator++()  // pre-increment, ++iter
        {
            // Increment offset and return this
            //std::cout << "v_iter<T>& operator++() " << std::endl;
            if(offset_x < get_max_x())
                ++offset_x;
            else
            {
                offset_x = 0;
                ++offset_y;
            }
            //std::cout << "iter2d :: operator++()\toffset = (" << offset_x << ", " << offset_y << ")\n"; 

            return (*this);
        }

        iter2d<T> operator++(int) // post-increment, iter++
        {
            // Create a copy, which we return. Increment the offset of this.
            //std::cout << "v_iter<T>& operator++(int) " << std::endl;
            iter2d<T> clone(*this);
            //std::cout << "iter2d :: operator++(int)\toffset = (" << offset_x << ", " << offset_y << ")\n"; 
            if(offset_x < get_max_x())
                ++offset_x;
            else
            {
                offset_x = 0;
                ++offset_y;
            }
            return (clone);
        }

        bool operator!= (const iter2d<T>& rhs)
        {
            //std::cout << "v_iter :: operator !=  : "; 
            if(get_offset_x() != rhs.get_offset_x() || get_offset_y() != rhs.get_offset_y())
            {
                //std::cout << "true" << std::endl;
                return(true);
            }
            else
            {
                //std::cout << "false" << std::endl;
                return(false);
            }
            
        }


        T& operator*()
        {
            //std::cout << "v_iter :: operator*: data at " << data_ref << ", offset = " << get_offset() << ", val = " << data_ref[get_offset()] << std::endl;
            return (data_ref[(get_offset_x() + get_sl().get_padx()) + get_offset_y()]);
        }
        

    private:
        T* data_ref;
        const slab_layout_t<T> sl;
        size_t offset_x;
        size_t offset_y;
        const bool transformed;
};


#endif // ITERATORS_H