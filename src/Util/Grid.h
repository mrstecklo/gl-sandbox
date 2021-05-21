#pragma once

#include <vector>

namespace Util {

template<class T, class Allocator = std::allocator<T> >
class Grid : private std::vector<T, Allocator> {
public:
    using Base = std::vector<T, Allocator>;
    using Base::value_type;
    using Base::allocator_type;
    using Base::size_type;
    using Base::difference_type;
    using Base::reference;
    using Base::const_reference;
    using Base::pointer;
    using Base::const_pointer;

    Grid() = default;

    explicit Grid(const Allocator& a) :
        Base(a) {}

    Grid(size_type width, size_type height, const T& value, const Allocator& alloc = Allocator()) : 
        Base(width * height, value, alloc),
        xsize(width)
    {}

    Grid(size_type width, size_type height, const Allocator& alloc = Allocator()) :
        Base(widht, * heighr, alloc)
        xsize(width)
    {}

    Grid(const Grid&) = default;
    Grid& operator=(const Grid&) = default;

    Grid(Grid&& other) :
        Base(std::move(other)),
        xsize(std::exchange(other.xsize, 0))
    {}

    Grid& operator=(Grid&& other)
    {
        Base::operator=(std::move(other));
        std::swap(xsize, other.xsize)
        return *this;
    }

    ~Grid() = default;

private:
    size_type xsize = 0;
};

template<class T>
class Row {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    

private:
    
};

} // namespace Util
