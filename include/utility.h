#ifndef UTILITY_H
#define UTILITY_H

#include "view.h"
#include <iomanip>
#include <sstream>

#if defined(__CUDACC__)
#define LAMBDACALLER __host__ __device__
#endif


#if !defined(__CUDACC__)
#define LAMBDACALLER
#endif

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
    void kernel_apply(T* data, F myfunc, const bounds_t bounds, const offset_t stride, const geometry_t<T> geom)
    {
        strided_view<T> view(data, bounds, stride);
        offset_t offset_kernel(thread_idx :: get_row(), thread_idx :: get_col());
        //printf("kernel_apply: bounds = (%lu, %lu, %lu, %lu)\n", bounds.get_nx(), bounds.get_pad_nx(), bounds.get_my(), bounds.get_pad_my());
        //printf("kernel_apply: stride= (%lu, %lu)\n", stride[0], stride[1]);
        //printf("kernel_apply: geom = (%f, %f, %f, %f)\n", geom.get_xleft(), geom.get_dx(), geom.get_ylo(), geom.get_dy());
        //printf("kernel_apply: offset = (%lu, %lu)\n", offset_kernel[0], offset_kernel[1]);

        if(bounds.contains(offset_kernel))
        {
            view[offset_kernel] = myfunc(view[offset_kernel], offset_kernel, geom);
            //view[offset_kernel] = T(threadIdx.x + 10 * blockIdx.x + 100 * threadIdx.y + 1000 * blockIdx.y);
        }
    }
} // End namespace device
#endif // __CUDACC__



namespace detail
{
#if defined (__CUDACC__)
    template<typename T, typename F>
    void impl_apply(vector2d<T, allocator_device>& vec, F myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_device<T>)
    {
        dim3 block{5, 5};
        dim3 grid{2, 2};
        device :: kernel_apply<<<grid, block>>>(vec.get_data(), myfunc, bounds, stride, geom);
    }
#endif //__CUDACC__


    template<typename T, typename F>
    void impl_apply(vector2d<T, allocator_host>& vec, F myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_host<T>)
    {
        strided_view<double> view(vec, bounds, stride);  
        for(auto& it : view.get_bounds())
        {
            view[it] = myfunc(view[it], it, geom());
        }
    }
}


namespace utility
{
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
    */


    template<typename T, template <typename> class allocator>
    inline void print(vector2d<T, allocator>& vec, const bounds_t bounds, const offset_t& stride)
    {
        strided_view<double> view(vec, bounds, stride);
        for(auto it : bounds)
        {
            std::cout << std::setw(10) << view[it] << "\t";
            if(it[1] == bounds.get_my() - 1)
                std::cout << std::endl;
        }    }

    template<typename T, template <typename> class allocator>
    inline void print(vector2d<T, allocator> vec, const bounds_t bounds, const offset_t& stride)
    {
        strided_view<double> view(vec, bounds, stride);
        for(auto it : bounds)
        {
            std::cout << std::setw(10) << view[it] << "\t";
            if(it[1] == bounds.get_my() - 1)
                std::cout << std::endl;
        }
    }
}

#endif //UTILITY_H