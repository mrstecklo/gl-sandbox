namespace Util {

class Noncopyable {
public:
    Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

class Nonmovable : private Noncopyable {
public:
    Nonmovable() = default;

private:
    Nonmovable(Nonmovable&&) = delete;
    Nonmovable& operator=(Nonmovable&&) = delete;
};

} // namespace Util