/*
 * Test iterators
 */


#include <iostream>
#include <algorithm>
#include <exception>
#include "include/myvector.h"


template<typename T>
void print(const vector<T>& vec)
{
    //for(auto v : vec)
    for(auto v = vec.begin(); v != vec.end(); v++)
        std::cout << *v << "\t";
    std::cout  << std::endl;
}


int main(void)
{
    vector<double> foo(10);
    print(foo);
}
