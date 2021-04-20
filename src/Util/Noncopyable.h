#pragma once

namespace Util {

class Noncopyable {
public:
    constexpr Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

class Nonmovable : private Noncopyable {
public:
    constexpr Nonmovable() = default;

private:
    Nonmovable(Nonmovable&&) = delete;
    Nonmovable& operator=(Nonmovable&&) = delete;
};

} // namespace Util
