#include <iostream>
#include <complex>

#include "include/vector2d.h"
#include "include/view.h"

int main(void)
{
    const size_t Nx{4};
    const size_t pad_nx{0};
    const size_t My{4};
    const size_t pad_my{2};
    
    const geometry_t geom(0.0, 0.125, 0.0, 0.125, 0.0, 0.0, bc_t::bc_)