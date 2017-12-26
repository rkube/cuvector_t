
template <typename T>
class view
{
    public:
        constexpr view(vector2d<T>& _vec, bounds_t _b) : data{_vec.get_data()}, bounds{_b} {};

    private:
        T* data;
        bounds_t bounds;
};

namespace
{
    template <typename T>
    constexpr T& view_access(T* data, const offset_t& idx, const offset_t& stride)
    {
        const size_t offset{idx[0] * stride[0] + idx[1]};
        return data[offset];
    }
}
