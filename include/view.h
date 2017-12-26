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
        template <typename U>
        strided_view(vector2d<U>& _vec, const bounds_t _b, const offset_t _s) :
            data{reinterpret_cast<T*>(_vec.get_data())}, 
            bounds{_b},
            stride{_s}
            {
                std::cout << "view :: view . stride = " << stride[0] << ", " << stride[1] << std::endl;
            };

        constexpr T& operator[](const offset_t& idx)
        {
            assert(get_bounds().contains(idx));
            return(view_access(data, idx, get_stride()));
        }

        constexpr bounds_t get_bounds() const {return(bounds);}
        offset_t get_stride() const {return(stride);}

    private:
        T* data;
        bounds_t bounds;
        offset_t stride;
};


