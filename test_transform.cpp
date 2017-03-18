/*
 * Test iterators
 */


#include <iostream>
#include <algorithm>
#include <exception>
#include "include/myvector.h"
#include "include/iterators.h"


template<typename T>
void print(const vector<T>& vec)
{
    size_t n{0};
    for(auto v : vec)
        std::cout << n++ << ": " << v << "\t";
    std::cout  << std::endl;
}


int main(void)
{
    // Create un-transformed vector with padding
    vector<double> foo(8, 2);
    print(foo);

    // Copy to new vector
    vector<double> foo_copy(foo);
    print(foo_copy);

    foo.set_transformed(true);

    for(auto& it : foo)
        it = 12.2;

    print(foo);
    foo_copy = foo;

    print(foo_copy);

    size_t n{0};
    for(auto i = foo.begin(); i != foo.end(); i++)
    {
        std::cout << n++ << ": " << i.get_offset() << std::endl;
    }

}
