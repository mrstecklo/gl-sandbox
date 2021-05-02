#pragma once

namespace Util {

template<class T>
struct PointT {
    T x;
    T y;

    bool operator==(const PointT<T>& r) const { return x == r.x && y == r.y; }
    bool operator!=(const PointT<T>& r) const { return !(*this == r); }

    const PointT operator+(const PointT<T>& r) const { return {x + r.x, y + r.y}; }
    const PointT operator-(const PointT<T>& r) const { return {x - r.x, y - r.y}; }

    PointT& operator+=(const PointT<T>& r) { x += r.x; y += r.y; return *this; }
    PointT& operator-=(const PointT<T>& r) { x -= r.x; y -= r.y; return *this; }
};

using Point = PointT<int>;
using PointD = PointT<double>;

} // namespace Util
