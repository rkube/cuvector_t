#ifndef INTERP_H
#define INTERP_H


#if defined(__CUDACC__)
#define CUDA_MEMBER __host__ __device__
#endif

#if !defined(__CUDACC__)
#define CUDA_MEMBER
#endif

template <typename T>
class bval_interpolator
{
    protected: 
        // Do not instantiate bval_interpolators directly, but only via derived classes
        /** 
          .. cpp:function:: CUDA_MEMBER bval_interpolator :: bval_interpolator(const T _bval)
        */
        CUDA_MEMBER bval_interpolator(const T bval_, const T dx_) noexcept : bval{bval_}, dx{dx_} {};
        
    public:
        /// Interpolate the value outside the domain given last value inside and deltax
        /// uval is the value just inside the domain, deltax the discretization distance
        CUDA_MEMBER virtual T operator()(const T uval) const {return(-40);};
        CUDA_MEMBER virtual ~bval_interpolator() {};

        CUDA_MEMBER constexpr T get_bval() const {return(bval);};
        CUDA_MEMBER constexpr T get_dx() const {return(dx);};
    private:
        // The value at the boundary
        const T bval;
        const T dx;
};


template <typename T>
class bval_interpolator_dirichlet_left : public bval_interpolator<T>
{
    public:
        using bval_interpolator<T>::get_bval;
        using bval_interpolator<T>::get_dx;
    
        CUDA_MEMBER bval_interpolator_dirichlet_left(const T bval_, const T dx_) noexcept : bval_interpolator<T>(bval_, dx_) {};
        CUDA_MEMBER ~bval_interpolator_dirichlet_left() {};
        
        CUDA_MEMBER virtual T operator()(const T uval) const {return(get_bval() * 2.0 - uval);} ;
};


template <typename T>
class bval_interpolator_dirichlet_right : public bval_interpolator<T>
{
    public:
        using bval_interpolator<T>::get_bval;
        using bval_interpolator<T>::get_dx;
        CUDA_MEMBER bval_interpolator_dirichlet_right(const T bval_, const T dx_) noexcept : bval_interpolator<T>(bval_, dx_) {};
        CUDA_MEMBER ~bval_interpolator_dirichlet_right() {};
        
        CUDA_MEMBER virtual T operator()(const T uval) const {return(get_bval() * 2.0 - uval);};
};


template <typename T>
class bval_interpolator_neumann_left : public bval_interpolator<T>
{
    public:
        using bval_interpolator<T>::get_bval;
        using bval_interpolator<T>::get_dx;
        CUDA_MEMBER bval_interpolator_neumann_left(const T bval_, const T dx_) noexcept : bval_interpolator<T>(bval_, dx_) {};
        CUDA_MEMBER ~bval_interpolator_neumann_left() {};

        CUDA_MEMBER virtual T operator()(const T uval) const {return(uval - get_dx() * get_bval());};
};


template <typename T>
class bval_interpolator_neumann_right : public bval_interpolator<T>
{
    public:
        using bval_interpolator<T>::get_bval;
        using bval_interpolator<T>::get_dx;
        CUDA_MEMBER bval_interpolator_neumann_right(const T bval_, const T dx_) noexcept : bval_interpolator<T>(bval_, dx_) {};
        CUDA_MEMBER ~bval_interpolator_neumann_right() {};
        
        CUDA_MEMBER virtual T operator()(const T uval) const {return(get_dx() * get_bval() + uval);};
};

#endif // INTERP_H