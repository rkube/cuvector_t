#include <iostream>
#include <iomanip>
#include <complex>

#include "include/vector2d.h"
#include "include/view.h"
#include "include/utility.h"

using cmplx_t = std::complex<double>;

#if defined(__CUDACC__)
using real_vec = vector2d<double, allocator_device>;
using cmplx_vec = vector2d<cmplx_t, allocator_device>;
#define LAMBDACALLER __host__ __device__

#endif

#if !defined(__CUDACC__)
using real_vec = vector2d<double, allocator_host>;
using cmplx_vec = vector2d<cmplx_t, allocator_host>;
#define LAMBDACALLER
#endif

//using host_vec = vector2d<double, allocator_host>;
using host_vec = vector2d<cmplx_t, allocator_host>;

int main(void)
{
    const size_t Nx{8};
    const size_t pad_nx{0};
    const size_t My{8};
    const size_t pad_my{2};
    
    const bounds_t bounds_full{Nx, pad_nx, My, pad_my};
    const bounds_t bounds_view{Nx, 0, My, 0};
    const offset_t stride_double{My + pad_my, 1};

    const geometry_t<double> geom(0.0, 0.125, 0.0, 0.125);
    const bvals_t<double> bvals(0.0, 0.0, bc_t::bc_dirichlet, bc_t::bc_dirichlet);

    cmplx_vec vec1(bounds_full);
    cmplx_vec vec2(bounds_full);

    utility :: apply(vec1, [=] LAMBDACALLER (cmplx_t input, const offset_t o, const geometry_t<double> geom) -> cmplx_t
                    {
                        //return(3.39);
                        return(geom.get_x(o));
                    }, bounds_view, stride_double, geom);

    utility :: apply(vec2, [=] LAMBDACALLER (cmplx_t input, const offset_t o, const geometry_t<double> geom) -> cmplx_t
                    {
                        //return(3.39);
                        return(geom.get_x(o));
                    }, bounds_view, stride_double, geom);


#ifdef __CUDACC__
    utility :: print(utility :: create_host_vector(vec1), bounds_view, stride_double);
#endif

#ifndef __CUDACC__
    utility :: print(vec1, bounds_view, stride_double);
#endif
 
    utility :: elementwise(vec1, vec2, [=] LAMBDACALLER (std::complex<double> lhs, std::complex<double> rhs) -> std::complex<double>
                            { return(lhs + rhs); }, bounds_view, stride_double);;

#ifdef __CUDACC__
    utility :: print(utility :: create_host_vector(vec1), bounds_view, stride_double);
#endif

#ifndef __CUDACC__
    utility :: print(vec1, bounds_view, stride_double);
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