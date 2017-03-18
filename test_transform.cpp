/*
 * Test iterators
 */


#include <iostream>
#include <algorithm>
#include <exception>
#include "include/myvector.h"
#include "include/iterators.h"


template<typename T>
void print(const vector2d<T>& vec)
{
    size_t n{0};
    for(auto v : vec)
        std::cout << n++ << ": " << v << "\t";
    std::cout  << std::endl;
}


int main(void)
{
    slab_layout_t<double> sl(1.0, 1.0, 4, 1, 4, 1);

    // Create un-transformed vector with padding
    vector2d<double> foo(sl);
    print(foo);
    
    // Copy to new vector
    vector2d<double> foo_copy(foo);
    print(foo_copy);

    foo.set_transformed(true);

    for(auto& it : foo)
        it = 12.2;

    print(foo);
    foo_copy = foo;

    print(foo_copy);

    //size_t n{0};
    //for(auto i = foo.begin(); i != foo.end(); i++)
    //{
    //    std::cout << n++ << ": " << i.get_offset() << std::endl;
    //}
    
}
