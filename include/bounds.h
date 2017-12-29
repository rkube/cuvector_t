#ifndef VIEW_H
#define VIEW_H

#include <initializer_list>
#include <array>
#include <cassert>

/*
 * A simplified implementation of an array_view.
 * Inspired by https://github.com/wardw/array_view
 */


// Forward declaration
class offset_t;
class bounds_t;
class bounds_iterator_t;


/*
 * Define a class that represents a multidimensional index to a linear memoryspace
 */
class offset_t
{
    public:
        offset_t(std::initializer_list<size_t> il) noexcept;
        constexpr offset_t(const offset_t& rhs) noexcept : offset{rhs.offset} {}

        constexpr size_t& operator[](size_t n) {return(offset[n]);};
        constexpr const size_t& operator[](size_t n) const {return(offset[n]);};

        // Offset arithmetic
        offset_t operator+() const noexcept {return(*this);}
        offset_t operator-() const noexcept
        {
            offset_t copy{*this};
            copy.offset[0] *= -1;
            copy.offset[1] *= -1;
            return(copy);
        }

        offset_t& operator+= (const offset_t);
        offset_t& operator-= (const offset_t);

    private:
        std::array<size_t, 2> offset;
};


offset_t :: offset_t(std::initializer_list<size_t> il) noexcept
{
    assert(il.size() == 2);
    std::copy(il.begin(), il.end(), offset.data());
}

// Offset arithmetic
offset_t& offset_t :: operator+= (const offset_t rhs)
{
    (*this)[0] += rhs[0];
    (*this)[1] += rhs[1];

    return(*this);
}

offset_t& offset_t :: operator-= (const offset_t rhs)
{
    assert(rhs[0] <= (*this)[0]);
    assert(rhs[1] <= (*this)[1]);

    (*this)[0] -= rhs[0];
    (*this)[1] -= rhs[1];

    return(*this);
}

// Free functions
bool operator==(const offset_t lhs, const offset_t rhs) noexcept
{
    return( (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) );
}

bool operator!=(const offset_t lhs, const offset_t rhs) noexcept
{
    return(!(lhs == rhs));
}

offset_t operator+(const offset_t lhs, const offset_t rhs) noexcept
{
    offset_t copy{lhs};
    return(copy += rhs);
}

offset_t operator-(const offset_t lhs, const offset_t rhs) noexcept
{
    offset_t copy{lhs};
    return(copy -= rhs);
}




/*
 * Type that defines bounds
 */

class bounds_t
{
    public:

        using iterator       = bounds_iterator_t;
        using const_iterator = bounds_iterator_t;

        constexpr bounds_t(const size_t Nx_, const size_t pad_nx_, const size_t My_, const size_t pad_my_) :
            Nx(Nx_), pad_nx(pad_nx_), My(My_), pad_my(pad_my_) 
            {}

        constexpr bounds_t(const bounds_t& rhs) : Nx(rhs.get_nx()), pad_nx(rhs.get_pad_nx()), My(rhs.get_my()), pad_my(rhs.get_pad_my()) 
            {}

        constexpr bool contains(const offset_t&) const noexcept;

        bounds_iterator_t begin() const noexcept;
        bounds_iterator_t end() const noexcept;

        // Element access
        constexpr size_t get_nx() const noexcept {return(Nx);}
        constexpr size_t get_pad_nx() const noexcept {return(pad_nx);}
        constexpr size_t nelem_x() const noexcept {return(Nx + pad_nx);}
        constexpr size_t get_my() const noexcept {return(My);}
        constexpr size_t get_pad_my() const noexcept {return(pad_my);}
        constexpr size_t nelem_y() const noexcept {return(My + pad_my);}

        constexpr size_t size() const noexcept
        {
            return ((Nx + pad_nx) * (My + pad_my));
        }

    private:
        const size_t Nx;
        const size_t pad_nx;
        const size_t My;
        const size_t pad_my;
};


// Check if an index is within bounds
constexpr bool bounds_t :: contains(const offset_t& idx) const noexcept
{
    if( (idx[0] < nelem_x()) && (idx[1] < nelem_y()) )
        return(true);
    
    return(false);
}

// Free functions

// Returns true if lhs and rhs describe the same bounds
constexpr bool operator==(const bounds_t& lhs, const bounds_t& rhs) noexcept
{
    if( (lhs.nelem_x() == rhs.nelem_x()) && (lhs.nelem_y() == rhs.nelem_y()) )
        return true;
    return false;
}

constexpr bool operator!=(const bounds_t& lhs, const bounds_t& rhs) noexcept
{
    return(!(lhs == rhs));
}


/*
 * Defines iteration over a bounds range
 */
class bounds_iterator_t
{
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type      = offset_t;
        using difference_type = size_t;
        using pointer         = const offset_t*;
        using reference       = const offset_t;

        bounds_iterator_t(const bounds_t& b_, offset_t o_ = {0, 0}) noexcept : bounds(b_), offset{o_} {};
        
        bool operator==(const bounds_iterator_t& rhs) const
        {
            return(offset == rhs.offset);
        }
        
        pointer operator->() const {return(&offset);}
        reference operator*() const {return(offset);}

        bounds_iterator_t operator++();
        bounds_iterator_t operator++(int);
        bounds_iterator_t& _setOffTheEnd();

        const offset_t& get_offset() const {return offset;}

    private:
        const bounds_t bounds;
        offset_t offset;
};

bounds_iterator_t bounds_t::begin() const noexcept
{
    return(bounds_iterator_t(*this));
}

bounds_iterator_t bounds_t::end() const noexcept
{
    bounds_iterator_t iter(*this);
    iter._setOffTheEnd();

    return(iter);
}

// Increments the offset and return this
// The last dimension (offset[1]) is contiguous in memory. Iterate over this dimension first.
bounds_iterator_t bounds_iterator_t::operator++()
{
    assert(offset[0] <= bounds.nelem_x());
    assert(offset[1] <= bounds.nelem_y());

    // See if we can increase the column index
    if(offset[1] < bounds.nelem_y() - 1)
    {
        // Increase the column
        ++offset[1];
    }
    // See if we can increase the row index
    else if(offset[0] < bounds.nelem_x() - 1)
    {
        // Increase the row. Reset column index to zero.
        offset[1] = 0;
        ++offset[0];
    }
    // If neither row or column can be increased, let the iterator run into the bounds
    else
    {
        _setOffTheEnd();
    }
    // Use asserts to avoid non-terminating loops.
    assert(offset[0] <= bounds.nelem_x() + 1);
    assert(offset[1] <= bounds.nelem_y() + 1);

    return (*this);
}

bounds_iterator_t bounds_iterator_t::operator++(int)
{
    bounds_iterator_t tmp{*this};
    ++(*this);
    return(tmp);
}

bounds_iterator_t& bounds_iterator_t::_setOffTheEnd()
{
    offset[0] = bounds.nelem_x() - 1;
    offset[1] = bounds.nelem_y();

    return(*this);
}


////////////////////////////////////////////////////////////////////////////////
// Free functions
////////////////////////////////////////////////////////////////////////////////

bool operator==(const bounds_iterator_t& lhs, const bounds_iterator_t& rhs)
{
    return(lhs.operator==(rhs));
}

bool operator!=(const bounds_iterator_t& lhs, const bounds_iterator_t& rhs)
{
    return(!(lhs.operator==(rhs)));
}




#endif //VIEW_H