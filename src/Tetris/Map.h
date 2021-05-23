#pragma once

#include "Util/Grid.h"
#include "Util/Point.h"
#include <random>

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
    Map(std::size_t width, std::size_t height) :
        grid(width, height),
        gen(std::random_device()())
    {}
    
    void Tick();


private:
    Figure Spawn();

    Util::Grid<int> grid;
    std::mt19937    gen;
};

} // namespace Tetris
