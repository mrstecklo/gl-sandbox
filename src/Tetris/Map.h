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

    CONSTEXPR14 Util::Point operator*();

    CONSTEXPR14 FigureConstIterator& operator++() { ++idx; return *this; }
    CONSTEXPR14 FigureConstIterator& operator--() { --idx; return *this; }
    CONSTEXPR14 FigureConstIterator  operator++(int) { auto old = *this; operator++(); return old; }
    CONSTEXPR14 FigureConstIterator  operator--(int) { auto old = *this; operator--(); return old; }

    CONSTEXPR14 FigureConstIterator& operator+=(std::size_t i) { idx += i; return *this; }
    CONSTEXPR14 FigureConstIterator& operator-=(std::size_t i) { idx -= i; return *this; }

    friend CONSTEXPR14 FigureConstIterator operator+(FigureConstIterator r, std::size_t i) { r += i; return r; }
    friend CONSTEXPR14 FigureConstIterator operator+(std::size_t i, const FigureConstIterator& r) { return r + i; } 
    friend CONSTEXPR14 FigureConstIterator operator-(FigureConstIterator r, std::size_t i) { r -= i; return r; }

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


    CONSTEXPR14 void RotateClockwise()        { rot = static_cast<Rotation>((rot + 1) % NUM_ROTATIONS); }
    CONSTEXPR14 void RotateCounterClockwise() { rot = static_cast<Rotation>((rot + NUM_ROTATIONS - 1) % NUM_ROTATIONS); }

    CONSTEXPR14 void MoveDown()  { pos.y -= 1; }
    CONSTEXPR14 void MoveUp()    { pos.y += 1; }
    CONSTEXPR14 void MoveRight() { pos.x += 1; }
    CONSTEXPR14 void MoveLeft()  { pos.x -= 1; }

    CONSTEXPR14 Util::Point operator[](std::size_t i) const { return pos + offset[type][i] + hotspot[type][rot]; }
    
    constexpr ConstIterator begin() const { return ConstIterator(this, 0); }
    constexpr ConstIterator end()   const { return ConstIterator(this, tetra); }

private:
    Util::Point pos;
    Type        type;
    Rotation    rot;

    static const Util::Point offset[NUM_TYPES][tetra];
    static const Util::Point hotspot[NUM_TYPES][NUM_ROTATIONS];
};

#ifdef FULL_CONSTEXPR
CONSTEXPR14 Util::Point FigureConstIterator::operator*() { return (*parent)[idx]; }
#endif

class Map {
public:
    enum class State {
        INIT,
        FIGURE,
        CLEAN,
        GRAVITY,
        END
    };

    enum class Input {
        NIL,
        DOWN,
        RIGHT,
        LEFT,
        CLOCKWISE,
        COUNTERCLOCK
    };

    enum class Cell : int {
        EMPTY,
        SOLID
    };

    using Container = Util::Grid<Cell>;

    Map(std::size_t width, std::size_t height);
    virtual ~Map() = default;

    State GetState() const { return state; }
    bool Check(const Util::Point& p) const { return grid[p] != Cell::EMPTY; }

protected:

    virtual void InitFigureImpl() {}
    virtual void SolidifyImpl() {}
    virtual void MoveFigureImpl(Input /* in */, const Figure& /* old */) {}
    virtual void MoveRestictedImpl() {}
    virtual void CleanRowImpl(std::size_t /* idx */) {}
    virtual void FallCellsImpl(std::size_t /* column */, std::size_t /* first */, std::size_t /* height */, std::size_t /* destination */) {}

    void Tick(Input in);

    const Figure& GetFigure() const { return figure; }

    bool IsFigureValid() const;
    bool IsFigureOutside() const;
    bool DoesFigureCollide() const;
    bool IsPointOutside(const Util::Point& p) const;
    bool DoesPointCollide(const Util::Point& p) const;

private:

    Figure Spawn();
    void MoveFigure(Input in);
    
    void Solidify();
    void Clean();
    void CleanRow(std::size_t idx);
    void Fall();
    void FallCells(std::size_t column, std::size_t first, std::size_t height, std::size_t destination);

    enum class GravityState {
        SEEK_EMPTY,
        SEEK_SOLID
    };

    bool IsRowFilled(std::size_t idx) const;

    Container       grid;
    std::mt19937    gen;
    State           state = State::INIT;
    Figure          figure{ {0,0}, Figure::Type::O };
};

} // namespace Tetris
