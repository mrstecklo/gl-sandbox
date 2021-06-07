#pragma once

namespace Util {

template<class T>
struct PointT {
    T x;
    T y;

    constexpr bool operator==(const PointT& r) const { return x == r.x && y == r.y; }
    constexpr bool operator!=(const PointT& r) const { return !(*this == r); }

    constexpr PointT& operator+=(const PointT& r) { x += r.x; y += r.y; return *this; }
    constexpr PointT& operator-=(const PointT& r) { x -= r.x; y -= r.y; return *this; }

    friend constexpr const PointT operator+(PointT r, const PointT& l) { r += l; return r; }
    friend constexpr const PointT operator-(PointT r, const PointT& l) { r -= l; return r; }
};

using Point = PointT<int>;
using PointD = PointT<double>;

} // namespace Util
