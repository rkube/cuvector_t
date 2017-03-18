#ifndef DATATYPES_H
#define DATATYPES_H


template <typename T>
class slab_layout_t
{
    public:
        slab_layout_t(const T _Lx, const T _Ly, const size_t _Nx, const size_t _padx, const size_t _My, const size_t _pady) :
            Lx(_Lx), Ly(_Ly), Nx(_Nx), pad_nx(_padx), My(_My), pad_my(_pady), dx(Lx / T(Nx)), dy(Ly / T(My))
            {};

        bool operator==(const slab_layout_t rhs)
        {
            if( (get_Lx() == rhs.get_Lx()) &&
                (get_Ly() == rhs.get_Ly()) &&
                (get_nx() == rhs.get_nx()) &&
                (get_padx() == rhs.get_padx()) &&
                (get_my() == rhs.get_my()) &&
                (get_pady() == rhs.get_pady()) )
                return(true);
            return(false);
        }

        T get_Lx() const {return(Lx);}
        T get_Ly() const {return(Ly);}
        size_t get_nx() const {return(Nx);}
        size_t get_padx() const {return(pad_nx);}
        size_t get_my() const {return(My);}
        size_t get_pady() const {return(pad_my);}
        T get_dx() const {return(dx);}
        T get_dy() const {return(dy);}

    private:
        const T Lx;
        const T Ly;
        const size_t Nx;
        const size_t pad_nx;
        const size_t My;
        const size_t pad_my;
        const T dx;
        const T dy;
};


#endif //DATATYPES_H