#pragma once

#include "Util/Grid.h"
#include "Util/Point.h"
#include <random>
#include <stdexcept>

namespace Tetris {

class Figure {
public:
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


    void RotateClockwise()        { rot = static_cast<Rotation>((rot + 1) % NUM_ROTATIONS); }
    void RotateCounterClockwise() { rot = static_cast<Rotation>((rot + NUM_ROTATIONS - 1) % NUM_ROTATIONS); }

    void MoveDown()  { pos.y += 1; }
    void MoveUp()    { pos.y -= 1; }
    void MoveRight() { pos.x += 1; }
    void MoveLeft()  { pos.x -= 1; }

    Util::Point operator[](std::size_t i) const { return pos + offset[type][i] + hotspot[type][rot]; }

private:
    Util::Point pos;
    Type        type;
    Rotation    rot;

    static const Util::Point offset[NUM_TYPES][tetra];
    static const Util::Point hotspot[NUM_TYPES][NUM_ROTATIONS];
};

class Map {
public:
    using Container = Util::Grid<int>;

    Map(std::size_t width, std::size_t height) :
        grid(width, height),
        gen(std::random_device()())
    {
        if(width > std::numeric_limits<int>::max() || height > std::numeric_limits<int>::max()) {
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

    const Container& GetGrid() const { return grid; }
    const Figure& GetFigure() const { return figure; }


private:
    Figure Spawn();
    void MoveFigure(Input in);
    bool IsFigureValid();
    bool DoesFigureCollide();
    bool IsPointOutside(const Util::Point& p);
    bool DoesPointCollide(const Util::Point& p);
    void ToInitState();

    Container       grid;
    std::mt19937    gen;
    State           state = State::INIT;
    Figure          figure{ {0,0}, Figure::Type::O };
};

} // namespace Tetris
