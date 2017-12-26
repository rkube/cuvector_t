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
    const size_t My21 = (My + pad_my) / 2;
    
    bounds_t bounds_stride(Nx, pad_nx, My, pad_my);
    bounds_t bounds_double(Nx, pad_nx, My, 0);
    bounds_t bounds_cmplx(Nx, pad_nx, My21, 0);

    offset_t stride_double{My + pad_my, 1};
    offset_t stride_cmplx{My21, 1};

    vector2d<double> vec(bounds_stride);

    strided_view<double> v_double(vec, bounds_double, stride_double);
    strided_view<std::complex<double>> v_cmplx(vec, bounds_cmplx, stride_cmplx);


    for(auto i1 = v_double.get_bounds().begin(); 
        i1 != v_double.get_bounds().end();
        i1++)
    {
        std::cout << "i1 = " << (*i1)[0] << ", " << (*i1)[1];
        std::cout << "\t" << v_double[*i1] << std::endl;
    }


    for(auto i1 = v_cmplx.get_bounds().begin();
        i1 != v_cmplx.get_bounds().end();
        i1++)
    {
        std::cout << "i1 = " << (*i1)[0] << ", " << (*i1)[1];
        std::cout << "\t" << v_cmplx[*i1] << std::endl;   
    }




    return(0);
}