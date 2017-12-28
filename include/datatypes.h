#ifndef DATATYPES_H
#define DATATYPES_H


template <typename T>
class slab_layout_t
{
    public:
        slab_layout_t(const size_t _Nx, const size_t _padx, const size_t _My, const size_t _pady) :
            Nx(_Nx), pad_nx(_padx), My(_My), pad_my(_pady)
            {};

        bool operator==(const slab_layout_t rhs)
        {
            if( (get_nx() == rhs.get_nx()) &&
                (get_padx() == rhs.get_padx()) &&
                (get_my() == rhs.get_my()) &&
                (get_pady() == rhs.get_pady()) )
                return(true);
            return(false);
        }

        size_t get_nx() const {return(Nx);}
        size_t get_padx() const {return(pad_nx);}
        size_t get_my() const {return(My);}
        size_t get_pady() const {return(pad_my);}

    private:
        const size_t Nx;
        const size_t pad_nx;
        const size_t My;
        const size_t pad_my;
};

enum class bc_t {bc_dirichlet, bc_neumann, bc_periodic, bc_null};

template <typename T>
class geometry_t
{
    public:
        geometry_t(const T xl_, const T dx_, const T yl_, const T dy_, const T bv_l_, const T bv_r_, const bc_t bc_l_, const bc_t bc_r_) :
            x_left(xl_), delta_x(dx_), y_lo(yl_), delta_y(dy_), bval_left(bv_l_), bval_right(bv_r_), bc_left(bc_l_), bc_right(bc_r_)
            {}
        
        inline T get_xleft() const {return x_left;};
        inline T get_dx() const {return delta_x;};
        inline T get_ylo() const {return y_lo;};
        inline T get_dy() const {return delta_y;};
        inline T get_bv_left() const {return bval_left;};
        inline T get_bv_right() const {return bval_right;};
        inline bc_t get_bc_left() const {return bc_left;};
        inline bc_t get_bc_right() const {return bc_right;};

    private:
        const T x_left;
        const T delta_x;
        const T y_lo;
        const T delta_y;
        const T bval_left;
        const T bval_right;
        const bc_t bc_left;
        const bc_t bc_right;
}


#endif //DATATYPES_H