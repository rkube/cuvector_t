#ifndef DATATYPES_H
#define DATATYPES_H

#include "bounds.h"

enum class bc_t {bc_dirichlet, bc_neumann, bc_periodic, bc_null};

#if defined(__CUDACC__)
#define CUDA_MEMBER __host__ __device__

struct thread_idx
{
    // Vector type as y-values consecutive. Those are the columns.
    static __device__ size_t get_col() {return(blockIdx.y * blockDim.y + threadIdx.y);}
    // Vector type as x-values strided with My+pad_my. Those are the rows.
    static __device__ size_t get_row() {return(blockIdx.x * blockDim.x + threadIdx.x);}
};


#endif

#if !defined(__CUDACC__)
struct dim3{int x; int y; int z;};
#define CUDA_MEMBER
#endif


template <typename T>
class geometry_t
{
    public:
        CUDA_MEMBER geometry_t(const T xl_ = 0, const T dx_ = 0, const T yl_ = 0, const T dy_ = 0) :
                x_left(xl_), delta_x(dx_), y_lo(yl_), delta_y(dy_)
                {}
        
        CUDA_MEMBER constexpr T get_xleft() const {return x_left;};
        CUDA_MEMBER constexpr T get_dx() const {return delta_x;};
        CUDA_MEMBER constexpr T get_ylo() const {return y_lo;};
        CUDA_MEMBER constexpr T get_dy() const {return delta_y;};

        CUDA_MEMBER T get_x(const offset_t o_) const
        {
            return(get_xleft() + o_[0] * get_dx());
        }

        CUDA_MEMBER T get_y(const offset_t o_) const
        {
            return(get_ylo() + o_[1] * get_dy());
        }

        CUDA_MEMBER bool operator==(const geometry_t rhs) const
        {
            if( (get_xleft() == rhs.get_xleft()) &&
                (get_dx() == rhs.get_dx()) &&
                (get_ylo() == rhs.get_ylo()) &&
                (get_dy() == rhs.get_dy()))
            {
                return(true);
            }
            return(false);
        }

    private:
        const T x_left;
        const T delta_x;
        const T y_lo;
        const T delta_y;

};


template <typename T>
class bvals_t
{
    public:
        CUDA_MEMBER bvals_t(const T bv_l_, const T bv_r_, const bc_t bc_l_, const bc_t bc_r_) :
            bval_left(bv_l_), bval_right(bv_r_), bc_left(bc_l_), bc_right(bc_r_)
        {}

        CUDA_MEMBER constexpr T get_bv_left() const {return bval_left;};
        CUDA_MEMBER constexpr T get_bv_right() const {return bval_right;};
        CUDA_MEMBER constexpr bc_t get_bc_left() const {return bc_left;};
        CUDA_MEMBER constexpr bc_t get_bc_right() const {return bc_right;};

    private:
        const T bval_left;
        const T bval_right;
        const bc_t bc_left;
        const bc_t bc_right;
};


#endif //DATATYPES_H