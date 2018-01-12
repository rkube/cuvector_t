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

    template <typename T, typename UnaryOp>
    __global__
    void kernel_apply(T* data, UnaryOp myfunc, const bounds_t bounds, const offset_t stride, const geometry_t<T> geom)
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


    template <typename T, typename BinaryOp>
    __global__
    void kernel_elementwise(T* lhs, T* rhs, BinaryOp myfunc, const bounds_t bounds, const offset_t stride)
    {
        strided_view<T> view_lhs(lhs, bounds, stride);
        strided_view<T> view_rhs(rhs, bounds, stride);

        offset_t offset_kernel(thread_idx :: get_row(), thread_idx :: get_col());

        if(bounds.contains(offset_kernel))
        {
            view_lhs[offset_kernel] = myfunc(view_lhs[offset_kernel], view_rhs[offset_kernel]);
        }
    }
} // End namespace device
#endif // __CUDACC__



namespace detail
{
#if defined (__CUDACC__)
    template<typename T, typename UnaryOp>
    void impl_apply(vector2d<T, allocator_device>& vec, UnaryOp myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_device<T>)
    {
        dim3 block{5, 5};
        dim3 grid{2, 2};
        device :: kernel_apply<<<grid, block>>>(vec.get_data(), myfunc, bounds, stride, geom);
    }

    template<typename T, typename BinaryOp>
    void impl_elementwise(vector2d<T, allocator_device>& lhs, 
                          vector2d<T, allocator_device>& rhs,
                          BinaryOp myfunc, const bounds_t& bounds, const offset_t& stride, allocator_device<T>)
    {
        dim3 block{5, 5};
        dim3 grid{2, 2};
        device :: kernel_elementwise<<<grid, block>>>(lhs.get_data(), rhs.get_data(), myfunc, bounds, stride);
    }
    
#endif //__CUDACC__


    template<typename T, typename UnaryOp>
    void impl_apply(vector2d<T, allocator_host>& vec, UnaryOp myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom, allocator_host<T>)
    {
        strided_view<T> view(vec, bounds, stride);  
        for(auto& it : bounds)
        {
            view[it] = myfunc(view[it], it, geom);
        }
    }

    template <typename T, typename BinaryOp>
    void impl_elementwise(vector2d<T, allocator_host>& lhs, vector2d<T, allocator_host>& rhs, BinaryOp myfunc, const bounds_t& bounds, const offset_t& stride, allocator_host<T>)
    {
        strided_view<T> view_lhs(lhs, bounds, stride);
        strided_view<T> view_rhs(rhs, bounds, stride);

        for(auto& it: bounds)
        {
            view_lhs[it] = myfunc(view_lhs[it], view_rhs[it]);
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

    template <typename T, template <typename> class allocator, typename UnaryOp>
    inline void apply(vector2d<T, allocator>& vec, UnaryOp myfunc, const bounds_t& bounds, const offset_t& stride, const geometry_t<T>& geom)
    {
        detail :: impl_apply(vec, myfunc, bounds, stride, geom, typename vector2d<T, allocator>::allocator_type{});
    }

    template <typename T, template <typename> class allocator, typename BinaryOp>
    void elementwise(vector2d<T, allocator>& lhs, vector2d<T, allocator>& rhs, BinaryOp myfunc, const bounds_t& bounds, const offset_t& stride)
    {
        assert(lhs.get_bounds().contains(bounds.end().get_offset()));
        assert(rhs.get_bounds().contains(bounds.end().get_offset()));

        detail :: impl_elementwise(lhs, rhs, myfunc, bounds, stride, typename vector2d<T, allocator>::allocator_type{});
    }


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

#if defined(__CUDACC__)
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
#endif
}

#endif //UTILITY_H