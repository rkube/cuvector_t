#include <iostream>
#include <iomanip>

#include "include/vector2d.h"
#include "include/view.h"
#include "include/utility.h"


#ifdef __CUDACC__
using real_vec = vector2d<double, allocator_device>;
#endif

#ifndef __CUDACC__
using real_vec = vector2d<double, allocator_host>;
#endif

using host_vec = vector2d<double, allocator_host>;

int main(void)
{
    const size_t Nx{8};
    const size_t pad_nx{0};
    const size_t My{8};
    const size_t pad_my{2};
    
    const geometry_t<double> geom(0.0, 0.125, 0.0, 0.125);
    const bvals_t<double> bvals(0.0, 0.0, bc_t::bc_dirichlet, bc_t::bc_dirichlet);

    const offset_t stride_double{My + pad_my, 1};

    bounds_t bounds(Nx, pad_nx, My, 0);
    //vector2d<double, allocator_host> vec1(bounds_t{Nx, pad_nx, My, pad_my});
    real_vec vec1(bounds_t{Nx, pad_nx, My, pad_my});

    //vector2d<double, allocator_host> vec2(bounds_t{Nx, pad_nx, My, pad_my});

    utility :: apply(vec1, [=] (double input, const offset_t o, const geometry_t<double> geom) -> double
                    {
                        return(geom.get_x(o));
                    }, bounds, stride_double, geom);

#ifdef __CUDACC__
    host_vec res(bounds_t{Nx, pad_nx, My, pad_my});
    utility :: print(utility :: create_host_vector(vec1), bounds, stride_double);
#endif

#ifndef __CUDACC__
    utility :: print(vec1, bounds, stride_double);
#endif

    /*
    strided_view<double, host_tag> view1(vec1, bounds, stride_double, geom);
    strided_view<double, host_tag> view2(vec2, bounds, stride_double, geom);

    utility :: apply(view1, [=] (double input, const offset_t o, const geometry_t<double> geom) -> double
                    {
                        return(geom.get_x(o));
                    }); 
    utility :: print(view1); 

    utility :: apply(view2, [=] (double input, const offset_t o, const geometry_t<double> geom) -> double
                    {
                        return(geom.get_x(o));
                    });

    utility :: elementwise(view1, view2, [=] (double lhs, double rhs, const offset_t o, geometry_t<double> geom) -> double
                    {
                        return(lhs + rhs);
                    });

    utility :: print(view1);
    */
    
    return(0.0);
}