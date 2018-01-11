#ifndef UTILITY_H
#define UTILITY_H

#include "view.h"
#include "policy.h"
#include <iomanip>
#include <sstream>


#if defined (__CUDACC__)
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

namespace 
{
    inline void gpuAssert(cudaError_t code, const char *file, int line)
    {   
        if (code != cudaSuccess)
        {   
            std::stringstream err_str;
            err_str << "GPUassert: " << cudaGetErrorString(code) << "\t file: " << file << ", line: " << line << "\n";
            throw gpu_error(err_str.str());
        }
    }
} // End anonymous namespace


namespace device
{

    template <typename T, typename F>
    __global__
    void kernel_apply(T* data, F myfunc)
    {

        std::array<double, 3> a{1.0, 2.0, 3.0};


        printf("kernel_apply: %f\t%f\t%f\n", a[0], a[1], a[2]);
        

    }
} // End namespace device
#endif // __CUDACC__



namespace detail
{
#if defined (__CUDACC__)
    template<typename T, typename F>
    void impl_apply(vector2d<T, allocator_device>& vec, F myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_device<T>)
    {
        std::cout << "Dispatching kernel_apply" << std::endl;
        device :: kernel_apply<<<1, 1>>>(vec.get_data(), myfunc);
    }
#endif //__CUDACC__

/*
    template<typename T, typename F>
    void impl_apply(strided_view<T, host_tag>& view, F myfunc, host_tag)
    {      
        for(auto& it : view.get_bounds())
        {
            view[it] = myfunc(view[it], it, view.get_geom());
        }  
    }
*/

    template<typename T, typename F>
    void impl_apply(vector2d<T, allocator_host>& vec, F myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_host<T>)
    {
        strided_view<double, host_tag> view(vec, bounds, stride, geom);  
        for(auto& it : view.get_bounds())
        {
            view[it] = myfunc(view[it], it, view.get_geom());
        }
    }

    template <typename T>
    void impl_print(vector2d<T, allocator_host>& vec, const bounds_t& bounds, const offset_t& stride, allocator_host<T>)
    {
        strided_view<double, host_tag> view(vec, bounds, stride, geometry_t<double>(0, 0, 0, 0));
        for(auto it : view.get_bounds())
        {
            std::cout << std::setw(10) << view[it] << "\t";
            if(it[1] == view.get_bounds().get_my() - 1)
                std::cout << std::endl;
        }
    }
}

 

namespace utility
{
    /*
    template <typename T, typename P, typename F>
    inline void apply(strided_view<T, P>& view, F myfunc)
    {
        detail :: impl_apply(view, myfunc, typename strided_view<T, P>::policy{});
    }
    */

#ifdef __CUDACC__
    template <typename T>
    vector2d<T, allocator_host> create_host_vector(vector2d<T, allocator_device>& src)
    {
        vector2d<T, allocator_host> res (src.get_bounds());
        gpuErrchk(cudaMemcpy(res.get_data(), src.get_data(), src.get_bounds().get_nelem() * sizeof(T), cudaMemcpyDeviceToHost));
        return(res);
    }

#endif

    template <typename T, template <typename> class allocator, typename F>
    inline void apply(vector2d<T, allocator>& vec, F myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom)
    {
        detail :: impl_apply(vec, myfunc, bounds, stride, geom, typename vector2d<T, allocator>::allocator_type{});
    }

    /*
    template <typename T, typename P, typename F>
    void elementwise(strided_view<T, P>& lhs, strided_view<T, P>& rhs, F myfunc)
    {
        assert(lhs.get_bounds() == rhs.get_bounds());
        {
            for(auto& it : lhs.get_bounds())
            {
                lhs[it] = myfunc(lhs[it], rhs[it], it, lhs.get_geom());
            }
        }
    }
    

    template<typename T, typename P>
    void print(strided_view<T, P>& view)
    {
        for(auto it : view.get_bounds())
        {
            std::cout << std::setw(10) << view[it] << "\t";
            if(it[1] == view.get_bounds().get_my() - 1)
                std::cout << std::endl;
        }
    }
    */


    template<typename T, template <typename> class allocator>
    inline void print(vector2d<T, allocator> vec, const bounds_t bounds, const offset_t& stride)
    {
        detail :: impl_print(vec, bounds, stride, typename vector2d<T, allocator> :: allocator_type{});
    }
}

#endif //UTILITY_H