#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include "iterators.h"

template <typename T> class v_iter;

template <typename T>
class vector
{
    using iterator = v_iter<T>;
    public:
        vector(const size_t);
        vector(const vector<T>&);
        ~vector();

        void print();
        size_t size() const {return (nelem);}

        iterator begin() const
        {
            return (v_iter<T>(this -> get_data(), get_nelem(), 0));
        }

        iterator end() const
        {
            return (v_iter<T>(this -> get_data(), get_nelem(), get_nelem()));
        }

        T& operator[](const size_t n)
        {
            if(n <= nelem)
            {
                return data[n];
            }
            else
                throw std::logic_error("foo");
        }

        T* get_data() const {return(data);}
        size_t get_nelem() const {return(nelem);}

    private:
        T* data;
        const size_t nelem;
};


template <typename T>
vector<T> :: vector(const size_t _n) : nelem(_n), data{new T[_n]}
{
    std::cout << "vector<T> :: vector(const size_t)" << std::endl;
    std::cout << "    data at " << data << std::endl; 
    std::cout << "    begin loop" << std::endl;
    //for(auto i = this -> begin(); i != this -> end(); i++)
    for(auto i : (*this))
    {
        i = -3.14;
        std::cout << "i = " << i <<  std::endl;
    }
    std::cout << "    end loop" << std::endl;
}


template <typename T>
vector<T> :: vector(const vector<T>& rhs) : nelem(rhs.size()), data{new T[rhs.size()]}
{
    std::cout << "vector<T> :: vector(const vector<T>&)" << std::endl;
    //auto i = this -> begin();
    //for (auto r : rhs)
    //    *i++ = r++;
    auto i = this -> begin();
    for(auto r = this -> begin(); r != this -> end(); ++r)
    {
        std::cout << "i = " << *i << ", r = " << *r << std::endl;
        *i = *r;
        ++i;
    }
}


template <typename T>
vector<T> :: ~vector()
{
    delete [] data;
}

#endif // MYVECTOR
