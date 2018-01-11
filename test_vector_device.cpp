#include <iostream>
#include "include/vector2d.h"


int main(void)
{
    const size_t Nx{4};
    const size_t pad_nx{0};
    const size_t My{8};
    const size_t pad_my{2};

    const bounds_t bounds(Nx, pad_nx, My, pad_my);

    vector2d<double, allocator_device> vec(bounds);


    return(0);
}