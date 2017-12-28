#include "vector2d.h"


// Members in an anonymous namespace are only visible in the local file scope
namespace
{
    template <typename T>
    constexpr T& view_access(T* data, const offset_t& idx, const offset_t& stride)
    {
        const size_t offset{idx[0] * stride[0] + idx[1]};
        return data[offset];
    }
}


template <typename T>
class strided_view
{
    public:

        using pointer       = T*;
        using reference     = T&;
        using value         = T;

        // Construct a strided view, given a vector, bounds, and stride
        template <typename U>
        strided_view(vector2d<U>& vec_, const bounds_t b_, const offset_t s_) :
            data{reinterpret_cast<T*>(vec_.get_data())}, 
            bounds{b_},
            stride{s_}
            {}

        // Construct a strided view given a data pointer, bounds, and stride
        template <typename U>
        strided_view(U* data_, const bounds_t b_, offset_t s_) :
            data{reinterpret_cast<T*>(data_)},
            bounds{b_},
            stride{s_}
            {}

        // Access element at an index
        constexpr T& operator[](const offset_t& idx)
        {
            assert(get_bounds().contains(idx));
            return(view_access(data, idx, get_stride()));
        }

        // Returns view on a section
        strided_view<T> section(const offset_t, const offset_t);

        constexpr bounds_t get_bounds() const {return(bounds);}
        offset_t get_stride() const {return(stride);}

    private:
        T* data;
        bounds_t bounds;
        offset_t stride;
};

template <typename T>
strided_view<T> strided_view<T> :: section(const offset_t origin, const offset_t window)
{
    std::cout << "strided_view<T> :: section..." << std::endl;
    
    offset_t w_minus_1{window - offset_t{1,1}};

    // Assert that the origin is a valid index
    assert(bounds.contains(origin));

    // Assert that the end of the section is a valid index.
    // Since the end of the section will checked against subtract one from the
    // section bounds. That is, check if 
    // origin + window <= bounds
    // not of
    // origin + window < bounds
    assert(bounds.contains(origin + (window - offset_t{1, 1})));
    // Construct a new bounds_t given the window.
    bounds_t tmp(window[0], bounds.get_pad_nx(), window[1], bounds.get_pad_my());
    return strided_view<T>(&(*this)[origin], tmp, stride);
}



