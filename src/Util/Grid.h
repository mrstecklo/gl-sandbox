#pragma once

#include "Point.h"

#include <vector>
#include <utility>

namespace Util {

template<class T>
class Row;

template<class T>
class ConstRow;

template<class T, class Allocator = std::allocator<T> >
class Grid : private std::vector<T, Allocator> {
public:
    using Base = std::vector<T, Allocator>;
    using typename Base::value_type;
    using typename Base::allocator_type;
    using typename Base::size_type;
    using typename Base::difference_type;
    using typename Base::reference;
    using typename Base::const_reference;
    using typename Base::pointer;
    using typename Base::const_pointer;

    Grid() = default;

    explicit Grid(const Allocator& a) :
        Base(a) {}

    Grid(size_type width, size_type height, const T& value, const Allocator& alloc = Allocator()) : 
        Base(width * height, value, alloc),
        xsize(width)
    {}

    Grid(size_type width, size_type height, const Allocator& alloc = Allocator()) :
        Base(width * height, alloc),
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
        std::swap(xsize, other.xsize);
        return *this;
    }

    ~Grid() = default;

    using Base::data;
    using Base::size;

    size_type width() const { return xsize; }
    size_type height() const { return (xsize > 0) ? (size() / xsize) : 0; }

    reference       operator[](const Point& p)       { return operator[](p.y)[p.x]; }
    const_reference operator[](const Point& p) const { return operator[](p.y)[p.x]; }

    Row<T>      operator[](size_type i);
    ConstRow<T> operator[](size_type i) const;

    Row<T>      begin()       {return operator[](0); }
    ConstRow<T> begin() const {return operator[](0); }

    Row<T>      end()       {return operator[](xsize); }
    ConstRow<T> end() const {return operator[](xsize); }
 
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

    constexpr Row(pointer ptr, size_type size) :
        p(ptr), s(size){}

    constexpr pointer data() const { return p; }
    constexpr reference operator[](size_type i) const { return p[i]; }

    constexpr pointer begin() const { return p; }
    constexpr pointer end()   const { return &p[s]; }
    
    constexpr Row& operator++() { p += s; return *this; }
    constexpr Row& operator--() { p -= s; return *this; }
    constexpr Row  operator++(int) { auto old = *this; operator++(); return old; }
    constexpr Row  operator--(int) { auto old = *this; operator--(); return old; }

    constexpr Row& operator+=(size_type i) { p += i * s; return *this; }
    constexpr Row& operator-=(size_type i) { p -= i * s; return *this; }

    friend constexpr Row operator+(Row<T> r, size_type i) { r += i; return r; }
    friend constexpr Row operator+(size_type i, const Row<T>& r) { return r + i; } 
    friend constexpr Row operator-(Row<T> r, size_type i) { r -= i; return r; }

private:
    pointer         p;
    size_type       s;
};

template<class T>
class ConstRow : private Row<T> {
public:
    using Base = Row<T>;
    using typename Base::value_type;
    using typename Base::size_type;
    using typename Base::difference_type;
    using typename Base::reference;
    using typename Base::const_reference;
    using typename Base::pointer;
    using typename Base::const_pointer;
    
    constexpr ConstRow(const_pointer ptr, size_type size) :
        Base(const_cast<pointer>(ptr), size) {}
    
    constexpr ConstRow(const Base& other) :
        Base(other) {}

    constexpr const_pointer data() const { return Base::data(); }
    constexpr const_reference operator[](size_type i) const { return Base::operator[](i); }

    constexpr const_pointer begin() const { return Base::begin(); }
    constexpr const_pointer end()   const { return Base::end(); }
    
    constexpr ConstRow& operator++() { return Base::operator++(); }
    constexpr ConstRow& operator--() { return Base::operator--(); }
    constexpr ConstRow  operator++(int) { return Base::operator++(0); }
    constexpr ConstRow  operator--(int) { return Base::operator--(0); }

    constexpr ConstRow& operator+=(size_type i) { Base::operator+=(i); return *this; }
    constexpr ConstRow& operator-=(size_type i) { Base::operator-=(i); return *this; }

    friend constexpr ConstRow operator+(ConstRow<T> r, size_type i) { r += i; return r; }
    friend constexpr ConstRow operator+(size_type i, const ConstRow<T>& r) { return r + i; } 
    friend constexpr ConstRow operator-(ConstRow<T> r, size_type i) { r -= i; return r; }

    friend constexpr bool operator<(const ConstRow& r, const ConstRow& l) { return r.data() < l.data(); }
    friend constexpr bool operator>(const ConstRow& r, const ConstRow& l) { return l < r; }
    friend constexpr bool operator<=(const ConstRow& r, const ConstRow& l) { return !(r > l); }
    friend constexpr bool operator>=(const ConstRow& r, const ConstRow& l) { return !(r < l); }
};

template<class T, class A>
Row<T> Grid<T, A>::operator[](size_type i) { return Row<T>(data(), xsize) + i; }
template<class T, class A>
ConstRow<T> Grid<T, A>::operator[](size_type i) const { return ConstRow<T>(data(), xsize) + i; }

} // namespace Util
