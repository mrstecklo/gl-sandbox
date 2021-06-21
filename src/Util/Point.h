#pragma once

#include "Defines.h"

namespace Util {

template<class T>
struct PointT {
    T x;
    T y;

    constexpr bool operator==(const PointT& r) const { return x == r.x && y == r.y; }
    constexpr bool operator!=(const PointT& r) const { return !(*this == r); }

    CONSTEXPR14 PointT& operator+=(const PointT& r) { x += r.x; y += r.y; return *this; }
    CONSTEXPR14 PointT& operator-=(const PointT& r) { x -= r.x; y -= r.y; return *this; }

    friend CONSTEXPR14 const PointT operator+(PointT r, const PointT& l) { r += l; return r; }
    friend CONSTEXPR14 const PointT operator-(PointT r, const PointT& l) { r -= l; return r; }
};

using Point = PointT<int>;
using PointD = PointT<double>;

} // namespace Util
