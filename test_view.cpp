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

    const bounds_t bounds_stride(Nx, pad_nx, My, pad_my);
    const bounds_t bounds_double(Nx, pad_nx, My, 0);
    const bounds_t bounds_cmplx(Nx, pad_nx, My21, 0);

    const geometry_t<double> geom(0.0, 1.0, 0.0, 1.0);

    const offset_t stride_double{My + pad_my, 1};
    const offset_t stride_cmplx{My21, 1};

    vector2d<double, allocator_host> vec(bounds_stride);

    strided_view<double> v_double(vec, bounds_double, stride_double, geom);
    strided_view<std::complex<double> > v_cmplx(vec, bounds_cmplx, stride_cmplx, geom);

    std::cout << "Viewing as double:" << std::endl;
    for(auto& i1 : v_double.get_bounds())
    {
        v_double[i1] = 2.7;
        std::cout << "i1 = " << i1[0] << ", " << i1[1];
        std::cout << "\t" << v_double[i1] << std::endl;      
    }

    std::cout << "Viewing as cmplx: " << std::endl;
    for(auto& i1: v_cmplx.get_bounds())
    {
        std::cout << "i1 = " << i1[0] << ", " << i1[1];
        std::cout << "\t" << v_cmplx[i1] << std::endl;
        v_cmplx[i1] = std::complex<double>(1.2, 2.4);
    }


    std::cout << "Testing section: " << std::endl;
    const offset_t origin{1, 1};
    const offset_t window{2, 2};
    strided_view<double> sec = v_double.section(origin, window);

    for(auto& i1: sec.get_bounds())
    {
        std::cout << "i1 = " << i1[0] << ", " << i1[1];
        std::cout << "\t" << sec[i1] << std::endl;
        sec[i1] = -1.0;
    }

    std::cout << "Viewing as double:" << std::endl;
    for(auto& i1 : v_double.get_bounds())
    {
        //v_double[i1] = 2.7;
        std::cout << "i1 = " << i1[0] << ", " << i1[1];
        std::cout << "\t" << v_double[i1] << std::endl;      
    }

    return(0);
}