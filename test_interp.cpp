#include <iostream>
#include <iomanip>

#include "include/vector2d.h"
#include "include/view.h"
#include "include/interp.h"

int main(void)
{
    const size_t Nx{8};
    const size_t pad_nx{0};
    const size_t My{8};
    const size_t pad_my{2};
    
    const geometry_t<double> geom(0.0, 0.125, 0.0, 0.125);
    const bvals_t<double> bvals(0.0, 0.0, bc_t::bc_dirichlet, bc_t::bc_dirichlet);

    const offset_t stride{My + pad_my, 1};
    const offset_t offset_left{0, 0};
    const offset_t offset_inner{1, 0};
    const offset_t offset_right{Nx - 1, 0};
    bounds_t bounds(Nx, 0, My, 0);
    vector2d<double> vec(bounds_t(Nx, pad_nx, My, pad_my));

    strided_view<double> view(vec, bounds, stride);

    strided_view<double> view_left = view.section(offset_left, offset_t{1, My}, geom, bvals);
    strided_view<double> view_inner = view.section(offset_inner, offset_t{Nx - 2, My});
    strided_view<double> view_right = view.section(offset_right, offset_t{1, My}, geom, bvals);

    offset_t i{0, 0};
    for(auto& i1: view_left.get_bounds())
    {
        // Use i1 for view_left and
        // i for view
        i = i1 + offset_left;
        std::cout <<  "i1 = " << std::setw(3) << i[0] << ", " << std::setw(3) << i[1] << ": ";
        std::cout << "x = " << std::setw(6) << geom.get_x(i) << ", y = " << std::setw(6) << geom.get_y(i1);
        std::cout << "\there = " << std::setw(10) << view.here(i);
        std::cout << "\tleft = " << std::setw(10) << view_left.left_ip(i1);
        std::cout << "\tright = " << std::setw(10) << view.right(i) << std::endl;      
    }

    std::cout << "=======================================================================================" << std::endl;

    for(auto& i1 : view_inner.get_bounds())
    {
        // Use (view, i) and (view_inner, i1)
        i = i1 + offset_inner;
        std::cout <<  "i1 = " << std::setw(3) << i[0] << ", " << std::setw(3) << i[1] << ": ";
        std::cout << "x = " << std::setw(6) << geom.get_x(i) << ", y = " << std::setw(6) << geom.get_y(i1);
        std::cout << "\there = " << std::setw(10) << view.here(i);
        if(i[1] < My)
            std::cout << "\tup = " << std::setw(10) << view.up(i);
        if(i[1] > 0)
            std::cout << "\tlo = " << std::setw(10) << view.lo(i);
        std::cout << "\tleft = " << std::setw(10) << view.left(i);
        std::cout << "\tright = " << std::setw(10) << view.right(i) << std::endl;

    }

    std::cout << "=======================================================================================" << std::endl;

    for(auto& i1: view_right.get_bounds())
    {
        // Use (view, i) and (view_right, i1)
        i = i1 + offset_right;
        std::cout <<  "i1 = " << std::setw(3) << i[0] << ", " << std::setw(3) << i[1] << ": ";
        std::cout << "x = " << std::setw(6) << geom.get_x(i) << ", y = " << std::setw(6) << geom.get_y(i1);
        std::cout << "\there = " << std::setw(10) << view.here(i);
        //std::cout << "\tup = " << std::setw(10) << view.up(i);
        //std::cout << "\tlo = " << std::setw(10) << view.lo(i);
        std::cout << "\tleft = " << std::setw(10) << view.left(i);
        std::cout << "\tright = " << std::setw(10) << view_right.right_ip(i1) << std::endl;      
    }

    return(0.0);
}