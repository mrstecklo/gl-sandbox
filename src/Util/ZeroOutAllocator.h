#pragma once

#include <memory>
#include <cstring>

namespace Util {

// Allocator adaptor that interposes construct() calls to
// convert value initialization into zero initialization
// Use with care!!! std::memset(ptr, 0, sizeof(U)) is called instead of default constructor
template <typename T, typename A = std::allocator<T>>
class ZeroOutAllocator : public A {
    typedef std::allocator_traits<A> a_t;
public:
    template <typename U> struct rebind {
        using other =
            ZeroOutAllocator<
            U, typename a_t::template rebind_alloc<U>
            >;
    };

    using A::A;

    template <typename U>
    void construct(U* ptr) noexcept
    {
        std::memset(ptr, 0, sizeof(U));
    }

    template <typename U, typename...Args>
    void construct(U* ptr, Args&&... args)
    {
        a_t::construct(static_cast<A&>(*this),
                       ptr, std::forward<Args>(args)...);
    }
};

} // namespace Util
