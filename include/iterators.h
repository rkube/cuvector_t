#ifndef ITERATORS_H
#define ITERATORS_H

template <typename T> 
class v_iter
{
    public:
        v_iter(T* _ref, const size_t _nelem, const size_t _offset) : nelem(_nelem), offset(_offset), data_ref(_ref) 
        {
            //std::cout << "v_iter(T*, const size_t, const size_t" << std::endl;
            //std::cout << "    data at " << data_ref << ", nelem = " << nelem << ", offset = " << offset << std::endl;
        }
        v_iter(T* _ref, const size_t _nelem) : nelem(_nelem), offset(0), data_ref(_ref) 
        {
            //std::cout << "v_iter(T*, const size_t nelem" << std::endl;
            //std::cout << "      data at " << data_ref << ", nelem = " << nelem << std::endl;
        }
    

        size_t get_offset() const {return(offset);}

        v_iter<T>& operator++()  // pre-increment, ++iter
        {
            // Increment offset and return this
            //std::cout << "v_iter<T>& operator++() " << std::endl;
            ++offset;
            return (*this);
        }

        v_iter<T> operator++(int) // post-increment, iter++
        {
            // Create a copy, which we return. Increment the offset of this.
            //std::cout << "v_iter<T>& operator++(int) " << std::endl;
            v_iter<T> clone(*this);
            ++offset;
            return (clone);
        }

        bool operator!= (const v_iter<T>& rhs)
        {
            //std::cout << "v_iter :: operator !=  : "; 
            if(get_offset() != rhs.get_offset())
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
            return (data_ref[get_offset()]);
        }

    private:
        const size_t nelem;
        size_t offset;
        T* data_ref;
};


#endif // ITERATORS_H