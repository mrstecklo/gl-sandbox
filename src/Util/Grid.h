#pragma once

#include "Point.h"

#include <vector>
#include <utility>

namespace Util {

template<class T>
class Row;

template<class T>
class ConstRow;

template<class RowType>
class GridIteratorBase;

template<class T>
using GridIterator = GridIteratorBase<Row<T> >;

template<class T>
using GridConstIterator = GridIteratorBase<ConstRow<T> >;

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

    using Iterator = GridIterator<T>;
    using ConstIterator = GridConstIterator<T>;

    Grid() = default;

    explicit Grid(const Allocator& a) :
        Base(a) {}

    Grid(size_type width, size_type height, const T& value, const Allocator& alloc = Allocator()) : 
        Base(width * height, value, alloc),
        xsize(width)
    {}

    Grid(size_type width, size_type height, const Allocator& alloc = Allocator()) :
#ifdef C14_VECTOR
        Base(width * height, alloc),
#else
        Base(width * height, T(), alloc),
#endif
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

    Iterator      begin()       {return Iterator(operator[](0)); }
    ConstIterator begin() const {return ConstIterator(operator[](0)); }

    Iterator      end()       {return Iterator(operator[](height())); }
    ConstIterator end() const {return ConstIterator(operator[](height())); }
 
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
    
    // TODO: these operators should be members of iterator, not row
    CONSTEXPR14 Row& operator++() { p += s; return *this; }
    CONSTEXPR14 Row& operator--() { p -= s; return *this; }
    CONSTEXPR14 Row  operator++(int) { auto old = *this; operator++(); return old; }
    CONSTEXPR14 Row  operator--(int) { auto old = *this; operator--(); return old; }

    CONSTEXPR14 Row& operator+=(size_type i) { p += i * s; return *this; }
    CONSTEXPR14 Row& operator-=(size_type i) { p -= i * s; return *this; }

    friend CONSTEXPR14 Row operator+(Row r, size_type i) { r += i; return r; }
    friend CONSTEXPR14 Row operator+(size_type i, const Row& r) { return r + i; } 
    friend CONSTEXPR14 Row operator-(Row r, size_type i) { r -= i; return r; }

    friend constexpr bool operator<(const Row& r, const Row& l) { return r.data() < l.data(); }
    friend constexpr bool operator>(const Row& r, const Row& l) { return l < r; }
    friend constexpr bool operator<=(const Row& r, const Row& l) { return !(r > l); }
    friend constexpr bool operator>=(const Row& r, const Row& l) { return !(r < l); }

    friend constexpr bool operator==(const Row& r, const Row& l) { return r.data() == l.data(); }
    friend constexpr bool operator!=(const Row& r, const Row& l) { return !(r == l); }

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
    
    CONSTEXPR14 ConstRow& operator++() { Base::operator++(); return *this; }
    CONSTEXPR14 ConstRow& operator--() { Base::operator--(); return *this; }
    CONSTEXPR14 ConstRow  operator++(int) { return Base::operator++(0); }
    CONSTEXPR14 ConstRow  operator--(int) { return Base::operator--(0); }

    CONSTEXPR14 ConstRow& operator+=(size_type i) { Base::operator+=(i); return *this; }
    CONSTEXPR14 ConstRow& operator-=(size_type i) { Base::operator-=(i); return *this; }

    friend CONSTEXPR14 ConstRow operator+(ConstRow r, size_type i) { r += i; return r; }
    friend CONSTEXPR14 ConstRow operator+(size_type i, const ConstRow& r) { return r + i; } 
    friend CONSTEXPR14 ConstRow operator-(ConstRow r, size_type i) { r -= i; return r; }

    friend constexpr bool operator<(const ConstRow& r, const ConstRow& l) { return r.data() < l.data(); }
    friend constexpr bool operator>(const ConstRow& r, const ConstRow& l) { return l < r; }
    friend constexpr bool operator<=(const ConstRow& r, const ConstRow& l) { return !(r > l); }
    friend constexpr bool operator>=(const ConstRow& r, const ConstRow& l) { return !(r < l); }

    friend constexpr bool operator==(const ConstRow& r, const ConstRow& l) { return r.data() == l.data(); }
    friend constexpr bool operator!=(const ConstRow& r, const ConstRow& l) { return !(r == l); }
};

template<class T, class A>
Row<T> Grid<T, A>::operator[](size_type i) { return Row<T>(data(), xsize) + i; }
template<class T, class A>
ConstRow<T> Grid<T, A>::operator[](size_type i) const { return ConstRow<T>(data(), xsize) + i; }

template<class RowType>
class GridIteratorBase {
public:

    using size_type = typename RowType::size_type;
    
    constexpr explicit GridIteratorBase(const RowType& r) :
        impl(r)
    {}

    template<class OtherRowType>
    constexpr GridIteratorBase(const GridIteratorBase<OtherRowType>& other) :
        impl(other.impl)
    {}

    constexpr const RowType& operator*() const { return impl; }
    constexpr const RowType* operator->() const { return &impl; }

    CONSTEXPR14 GridIteratorBase& operator++() { ++impl; return *this; }
    CONSTEXPR14 GridIteratorBase& operator--() { --impl; return *this; }
    CONSTEXPR14 GridIteratorBase  operator++(int) { return GridIterarorBase(impl++); }
    CONSTEXPR14 GridIteratorBase  operator--(int) { return GridIterarorBase(impl--); }

    CONSTEXPR14 GridIteratorBase& operator+=(size_type i) { impl += i; return *this; }
    CONSTEXPR14 GridIteratorBase& operator-=(size_type i) { impl -= i; return *this; }

    friend CONSTEXPR14 GridIteratorBase operator+(GridIteratorBase r, size_type i) { r += i; return r; }
    friend CONSTEXPR14 GridIteratorBase operator+(size_type i, const GridIteratorBase& r) { return r + i; } 
    friend CONSTEXPR14 GridIteratorBase operator-(GridIteratorBase r, size_type i) { r -= i; return r; }

    friend constexpr bool operator<(const GridIteratorBase& r, const GridIteratorBase& l) { return r.impl < l.impl; }
    friend constexpr bool operator>(const GridIteratorBase& r, const GridIteratorBase& l) { return l < r; }
    friend constexpr bool operator<=(const GridIteratorBase& r, const GridIteratorBase& l) { return !(r > l); }
    friend constexpr bool operator>=(const GridIteratorBase& r, const GridIteratorBase& l) { return !(r < l); }

    friend constexpr bool operator==(const GridIteratorBase& r, const GridIteratorBase& l) { return r.impl == l.impl; }
    friend constexpr bool operator!=(const GridIteratorBase& r, const GridIteratorBase& l) { return !(r == l); }


private:
    RowType impl;
};

} // namespace Util
