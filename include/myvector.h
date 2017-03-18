#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <cassert>

template <typename T> class v_iter;

template <typename T>
class vector
{
    public:
        using iterator_t = v_iter<T>;
        vector(const size_t, const size_t);
        vector(const vector<T>&);
        ~vector();

        size_t size() const {return (get_nelem() + get_pad());}

        iterator_t begin() const
        {
            return (iterator_t(this -> get_data(), get_nelem(), 0));
        }

        iterator_t end() const
        {
            // If the vector is transformed the end of the range is get_nelem() + get_pad().
            // If the vector is not transformed the end of the range is get_nelem()
            return(iterator_t(this -> get_data(),
                              get_transformed() ? get_nelem() + get_pad() : get_nelem(),
                              get_transformed() ? get_nelem() + get_pad() : get_nelem()));
        }


        void operator= (const vector<T>& rhs);

        T* get_data() const { return(data); }
        size_t get_nelem() const { return(nelem); }
        size_t get_pad() const { return(pad); }

        bool get_transformed() const { return(transformed); }
        void set_transformed(bool tr) { transformed = tr; }

    private:
        const size_t nelem;
        const size_t pad;
        bool transformed;
        T* data;
};


template <typename T>
vector<T> :: vector(const size_t _n, const size_t _pad) : nelem(_n), pad(_pad), transformed(false), data{new T[_n + _pad]}
{
    // Use reference here!
    for(auto& i : (*this))
        i = -3.14;
}


template <typename T>
vector<T> :: vector(const vector<T>& rhs) : 
    nelem(rhs.get_nelem()), 
    pad(rhs.get_pad()), 
    transformed(rhs.get_transformed()), 
    data{new T[rhs.size()]}
{
    auto r = rhs.begin();
    for(auto& i : (*this))
    {
        i = *r++;
    }
}


template <typename T>
void vector<T> :: operator=(const vector<T>& rhs)
{
    assert(get_nelem() == rhs.get_nelem());
    assert(get_pad() == rhs.get_pad());

    set_transformed(rhs.get_transformed());
    auto r = rhs.begin();
    for(auto& i : (*this))
    {
        i = *r++;
    }

}

template <typename T>
vector<T> :: ~vector()
{
    delete [] data;
}

#endif // MYVECTOR_H
