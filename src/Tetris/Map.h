#pragma once

#include "Util/Grid.h"
#include "Util/Point.h"
#include <random>
#include <stdexcept>

namespace Tetris {

class Figure;

class FigureConstIterator {
public:
    constexpr explicit FigureConstIterator(const Figure* f, std::size_t  index) :
        parent(f),
        idx(index)
    {}

    constexpr Util::Point operator*();

    constexpr FigureConstIterator& operator++() { ++idx; return *this; }
    constexpr FigureConstIterator& operator--() { --idx; return *this; }
    constexpr FigureConstIterator  operator++(int) { auto old = *this; operator++(); return old; }
    constexpr FigureConstIterator  operator--(int) { auto old = *this; operator--(); return old; }

    constexpr FigureConstIterator& operator+=(std::size_t i) { idx += i; return *this; }
    constexpr FigureConstIterator& operator-=(std::size_t i) { idx -= i; return *this; }

    friend constexpr FigureConstIterator operator+(FigureConstIterator r, std::size_t i) { r += i; return r; }
    friend constexpr FigureConstIterator operator+(std::size_t i, const FigureConstIterator& r) { return r + i; } 
    friend constexpr FigureConstIterator operator-(FigureConstIterator r, std::size_t i) { r -= i; return r; }

    friend constexpr bool operator<(const FigureConstIterator& r, const FigureConstIterator& l) { return r.idx < l.idx; }
    friend constexpr bool operator>(const FigureConstIterator& r, const FigureConstIterator& l) { return l < r; }
    friend constexpr bool operator<=(const FigureConstIterator& r, const FigureConstIterator& l) { return !(r > l); }
    friend constexpr bool operator>=(const FigureConstIterator& r, const FigureConstIterator& l) { return !(r < l); }

    friend constexpr bool operator==(const FigureConstIterator& r, const FigureConstIterator& l) { return r.idx == l.idx; }
    friend constexpr bool operator!=(const FigureConstIterator& r, const FigureConstIterator& l) { return !(r == l); }

private:
    const Figure* parent;
    std::size_t idx;
};

class Figure {
public:
    using ConstIterator = FigureConstIterator;

    constexpr static int tetra = 4;

    enum Type : int {
        O,
        T,
        S,
        Z,
        L,
        J,
        I,

        NUM_TYPES
    };

    enum Rotation : int {
        INIT,
        CLOCKWISE,
        FLIP,
        COUNTERCLOCK,

        NUM_ROTATIONS
    };

    constexpr Figure(const Util::Point p, Type t, Rotation r = INIT) :
        pos(p), type(t), rot(r) {}


    constexpr void RotateClockwise()        { rot = static_cast<Rotation>((rot + 1) % NUM_ROTATIONS); }
    constexpr void RotateCounterClockwise() { rot = static_cast<Rotation>((rot + NUM_ROTATIONS - 1) % NUM_ROTATIONS); }

    constexpr void MoveDown()  { pos.y += 1; }
    constexpr void MoveUp()    { pos.y -= 1; }
    constexpr void MoveRight() { pos.x += 1; }
    constexpr void MoveLeft()  { pos.x -= 1; }

    constexpr Util::Point operator[](std::size_t i) const { return pos + offset[type][i] + hotspot[type][rot]; }
    
    constexpr ConstIterator begin() const { return ConstIterator(this, 0); }
    constexpr ConstIterator end()   const { return ConstIterator(this, tetra); }

private:
    Util::Point pos;
    Type        type;
    Rotation    rot;

    static const Util::Point offset[NUM_TYPES][tetra];
    static const Util::Point hotspot[NUM_TYPES][NUM_ROTATIONS];
};

constexpr Util::Point FigureConstIterator::operator*() { return (*parent)[idx]; }

class Grid {
public:

    Grid(std::size_t width, std::size_t height) :
        flags(width, height) {}

    virtual ~Grid() = default;

    void Set(const Util::Point& p, bool value)
    {
        flags[p] = value ? 1 : 0;
        SetImpl(p, value);
    }

    bool Check(const Util::Point& p) const { return flags[p] != 0; }

    std::size_t width() const { return flags.width(); }
    std::size_t height() const { return flags.height(); }

protected:
    virtual void SetImpl(const Util::Point& p, bool value) {}

private:
    Util::Grid<int> flags;
};

class Map {
public:
    Map(Grid& g) :
        grid(g),
        gen(std::random_device()())
    {
        if(grid.width() > std::numeric_limits<int>::max() || grid.height() > std::numeric_limits<int>::max()) {
            throw std::runtime_error("Map::Map. Dimensions are too big");
        }
    }

    enum class State {
        INIT,
        FIGURE,
        END
    };

    enum class Input {
        NIL,
        DOWN,
        RIGHT,
        LEFT,
        ROTATE
    };
    
    void Tick(Input in);
    State GetState() const { return state; }

    const Figure& GetFigure() const { return figure; }


private:
    Figure Spawn();
    void MoveFigure(Input in);
    bool IsFigureValid();
    bool DoesFigureCollide();
    bool IsPointOutside(const Util::Point& p);
    bool DoesPointCollide(const Util::Point& p);
    void ToInitState();

    Grid&           grid;
    std::mt19937    gen;
    State           state = State::INIT;
    Figure          figure{ {0,0}, Figure::Type::O };
};

} // namespace Tetris
