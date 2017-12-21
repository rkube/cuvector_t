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
            //std::cout << "iter2d(T*, const size_t, const size_t" << std::endl;
            //std::cout << "    data at " << data_ref <<  ", offset_x = " << offset_x << ", offset_y = " << offset_y << std::endl;
        }
        iter2d(T* _ref, const slab_layout_t<T> _sl, bool _trans) : data_ref(_ref), sl(_sl), offset_x(0), offset_y(0), transformed(_trans)
        {
            //std::cout << "iter2d(T*, const size_t nelem" << std::endl;
            //std::cout << "    data at " << data_ref <<  ", offset_x = " << offset_x << ", offset_y = " << offset_y << std::endl;
        }
    
        size_t get_offset_x() const {return(offset_x);}
        size_t get_offset_y() const {return(offset_y);}

        const slab_layout_t<T> get_sl() const { return(sl); }

        size_t get_max_x() const {return(transformed ? get_sl().get_nx() + get_sl().get_padx() : get_sl().get_nx()); }
        size_t get_max_y() const {return(transformed ? get_sl().get_my() + get_sl().get_pady() : get_sl().get_my()); }


        iter2d<T>& operator++()  // pre-increment, ++iter
        {
            // Increment offset and return this
            assert(offset_x <= get_max_x());
            assert(offset_y <= get_max_y());
            //std::cout << "iter2d<T>& operator++() input:";
            //std::cout << "\toffset_x = " << offset_x;
            //std::cout << "\toffset_y = " << offset_y << std::endl;

            // See if we can increase the column index
            if(offset_x < get_max_x() - 1)
            {
                // Increase the column
                
                ++offset_x;
            }
            else
            {
                // See if we can increase the row index
                if(offset_y < get_max_y() - 1)
                {
                    // Increase the row. Reset column index to zero.
                    offset_x = 0;
                    ++offset_y;
                }
                else
                {
                    ++offset_x;
                    ++offset_y;
                }
            }
            //std::cout << "\titer2d :: operator++()\t exiting with offset = (" << offset_x << ", " << offset_y << ")" << std::endl;

            // Use asserts to avoid non-terminating loops.
            assert(offset_x <= get_max_x() + 1);
            assert(offset_y <= get_max_y() + 1);

            return (*this);
        }


        /*
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
        */

        bool operator!= (const iter2d<T>& rhs)
        {
            if(get_offset_x() != rhs.get_offset_x() || get_offset_y() != rhs.get_offset_y())
            {
                return(true);
            }
            else
            {
                return(false);
            }
            
        }

        T& operator*()
        {
            return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * get_offset_y() + get_offset_x()]);
        }

        T& here()
        {
            return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * get_offset_y() + get_offset_x()]);
        }
        

        T& right()
        {
            if(get_offset_x() < get_sl().get_nx() + get_sl().get_padx() - 1)
                return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * get_offset_y() + get_offset_x() + 1]);
            else
                return(T(-42.0));
        }

        T& left()
        {
            if(get_offset_x() > 0)
                return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * get_offset_y() + get_offset_x() + 1]);
            else
                return(T(-42.0));
        }

        T& up()
        {
            if(get_offset_y() < get_sl().get_my() + get_sl().get_pad_y() - 1)
                return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * (get_offset_y() + 1) + get_offset_x() + 1]);
            else
                return(T(-24.0));
        }

        T& down()
        {
            if(get_offset_y() > 0)
                return(data_ref[(get_sl().get_nx() + get_sl().get_padx()) * (get_offset_y() - 1) + get_offset_x() + 1]);
            else
                return(T(-24.0));
        }

    private:

        // Pointer to the data
        T* data_ref;
        // Stores the geometry and layout of the data
        const slab_layout_t<T> sl;
        // Gives the current column index
        size_t offset_x;
        // Gives the current row index
        size_t offset_y;
        // If true, iterate over padding data
        // If false, do not iterate over padding data
        const bool transformed;
};


#endif // ITERATORS_H