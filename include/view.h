#include "vector2d.h"
#include "datatypes.h"
#include "interp.h"


#ifndef VIEW_H
#define VIEW_H

#if defined(__CUDACC__)
#define CUDA_MEMBER __host__ __device__
#endif

#if !defined(__CUDACC__)
#define CUDA_MEMBER
#endif


// Members in an anonymous namespace are only visible in the local file scope
namespace
{
    template <typename T>
    CUDA_MEMBER constexpr T& view_access(T* data, const offset_t& idx, const offset_t& stride)
    {
        return data[idx[0] * stride[0] + idx[1]];
    }
}


template <typename T>
class strided_view
{
    public:
        using pointer   = T*;
        using reference = T&;
        using value     = T;

        // Construct a strided view, given a vector, bounds, and stride
        template <typename U, template<typename> class allocator>
        CUDA_MEMBER strided_view(vector2d<U, allocator>& vec_, const bounds_t b_, const offset_t s_) :
            data{reinterpret_cast<T*>(vec_.get_data())}, 
            bounds{b_},
            stride{s_},
            geom{0.0, 0.0, 0.0, 0.0},
            gp_interpolator_left{nullptr},
            gp_interpolator_right{nullptr}
            {}

        // Construct a strided view given a data pointer, bounds, and stride
        template <typename U>
        CUDA_MEMBER strided_view(U* data_, const bounds_t b_, offset_t s_) :
            data{reinterpret_cast<T*>(data_)},
            bounds{b_},
            stride{s_},
            geom{0.0, 0.0, 0.0, 0.0},
            gp_interpolator_left{nullptr},
            gp_interpolator_right{nullptr}           
            {}

        template <typename U>
        CUDA_MEMBER strided_view(U* data_, const bounds_t b_, const offset_t s_, const geometry_t<double> g_, const bvals_t<U> bv_) :
                    strided_view(data_, b_, s_, g_)
            {
                std::cout << "strided_view :: strided_view(interpolating)" << std::endl;
                switch(bv_.get_bc_left())
                {
                    case bc_t::bc_dirichlet:
                        gp_interpolator_left = new bval_interpolator_dirichlet_left<T>(bv_.get_bv_left(), geom.get_dx());
                        break;

                    case bc_t::bc_neumann:
                        gp_interpolator_left = new bval_interpolator_neumann_left<T>(bv_.get_bv_left(), geom.get_dx());
                        break;
                    case bc_t::bc_periodic:
                    case bc_t::bc_null:
                        break;
                }
        
                switch(bv_.get_bc_right())
                {
                    case bc_t::bc_dirichlet:
                        gp_interpolator_right = new bval_interpolator_dirichlet_right<T>(bv_.get_bv_right(), geom.get_dx());
                        break;

                    case bc_t::bc_neumann:
                        gp_interpolator_right = new bval_interpolator_neumann_right<T>(bv_.get_bv_right(), geom.get_dx());
                        break;
                    case bc_t::bc_periodic:
                    case bc_t::bc_null:
                        break;
                }
            }

        CUDA_MEMBER ~strided_view() noexcept
        {
            if(gp_interpolator_left != nullptr)
            {
                delete gp_interpolator_left;
            }
            if(gp_interpolator_right != nullptr)
            {
                delete gp_interpolator_right;
            }
        }


        // Access element at an index
        CUDA_MEMBER constexpr reference operator[](const offset_t& idx)
        {
            assert(get_bounds().contains(idx));
            return(view_access(data, idx, get_stride()));
        }

        // Read-only access to current element
        CUDA_MEMBER constexpr value here(const offset_t& idx)
        {            
            return(view_access(data, idx, get_stride()));
        }

        // Read-only access to element above
        CUDA_MEMBER constexpr value up(const offset_t& idx)
        {
            return(view_access(data, idx + offset_t{0, 1}, get_stride()));
        }


        // Read-only access to element below
        CUDA_MEMBER constexpr value lo(const offset_t& idx)
        {
            return(view_access(data, idx - offset_t{0, 1}, get_stride()));
        }

        // Read-only access to left element
        CUDA_MEMBER constexpr value left(const offset_t& idx)
        {
            return(view_access(data, idx - offset_t{1, 0}, get_stride()));
        }

        CUDA_MEMBER constexpr value left_ip(const offset_t& idx)
        {
            assert(gp_interpolator_left != nullptr);
            return((*gp_interpolator_left)(view_access(data, idx, get_stride())));
        }

        // Read-only access to right element
        CUDA_MEMBER constexpr value right(const offset_t& idx)
        {
            return(view_access(data, idx + offset_t{1, 0}, get_stride()));
        }

        CUDA_MEMBER constexpr value right_ip(const offset_t& idx)
        {
            assert(gp_interpolator_right != nullptr);
            return((*gp_interpolator_right)(view_access(data, idx, get_stride())));
        }

        // Returns view on a section
        CUDA_MEMBER strided_view<T> section(const offset_t, const offset_t);
        CUDA_MEMBER strided_view<T> section(const offset_t, const offset_t, const geometry_t<T>&, const bvals_t<T>&);

        CUDA_MEMBER constexpr const bounds_t& get_bounds() const {return(bounds);}
        CUDA_MEMBER const offset_t& get_stride() const {return(stride);}
        CUDA_MEMBER const geometry_t<double> get_geom() const {return(geom);}

    private:
        T* data;
        bounds_t bounds;
        offset_t stride;
        geometry_t<double> geom;
        bval_interpolator<T>* gp_interpolator_left;
        bval_interpolator<T>* gp_interpolator_right;
};

template <typename T>
CUDA_MEMBER 
strided_view<T> strided_view<T> :: section(const offset_t origin, const offset_t window)
{   
    // Assert that the origin is a valid index
    assert(bounds.contains(origin));

    // Assert that the end of the section is a valid index.
    // Since the end of the section will checked against subtract one from the
    // section bounds. That is, check if 
    // origin + window <= bounds
    // not of
    // origin + window < bounds
    //assert(bounds.contains(origin + w_minus_1));
    assert(bounds.contains(window - offset_t{1,1}));

    //bounds_t tmp(window[0], bounds.get_pad_nx(), window[1], bounds.get_pad_my());
    // Construct a new bounds_t given the window.
    return strided_view<T>(&(*this)[origin], 
                           bounds_t(window[0], bounds.get_pad_nx(), window[1], bounds.get_pad_my()), 
                           get_stride(), get_geom());
}

template <typename T>
CUDA_MEMBER 
strided_view<T> strided_view<T> :: section(const offset_t origin, const offset_t window, const geometry_t<T>& geom, const bvals_t<T>& bvals)
{
    assert(bounds.contains(origin));
    assert(bounds.contains(window - offset_t{1,1}));
    std::cout << "strided_view<T> :: section with IP" << std::endl;
    return strided_view<T>(&(*this)[origin], 
                              bounds_t(window[0], bounds.get_pad_nx(), window[1], bounds.get_pad_my()), 
                              stride, geom, bvals);

}

#endif //VIEW_H