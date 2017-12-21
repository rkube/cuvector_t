/*
 * Test iterators
 */


#include <iostream>
#include <iomanip>
#include <algorithm>  
#include <exception>
#include "include/myvector.h"
#include "include/iterators.h"


template<typename T>
void print(const vector2d<T>& vec)
{
    size_t n{0};
    //for(auto v : vec)
    for(auto v = vec.begin(); v != vec.end(); ++v)
    {
        std::cout << std::setw(10) << v.here() << "\t";
        if (v.get_offset_x() == (vec.get_transformed() ? vec.get_sl().get_nx() + vec.get_sl().get_padx() 
                                                       : vec.get_sl().get_nx()) - 1)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}


int main(void)
{
    slab_layout_t<double> sl(1.0, 1.0, 8, 0, 8, 2);

    // Create un-transformed vector with padding
    vector2d<double> foo(sl);

    std::cout << "Printing foo " << std::endl;
    print(foo);

    std::cout << "=======================================================================================" << std::endl;

    foo.set_transformed(true);

    for(auto& it : foo)
        it = 12.2;

    print(foo);
    //foo_copy = foo;

    //print(foo_copy);

    std::cout << "=======================================================================================" << std::endl;
}
