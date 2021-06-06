#include "Map.h"

#include <iostream>

namespace Tetris {

static_assert(
    Figure::O == 0 &&
    Figure::T == 1 &&
    Figure::S == 2 &&
    Figure::Z == 3 &&
    Figure::L == 4 &&
    Figure::J == 5 &&
    Figure::I == 6, "You ruined offset and hotspot arrays");

const Util::Point Figure::offset[NUM_TYPES][tetra] = {
    {{0, 0}, {0, 1}, {1, 1}, {1, 0}},   // O
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},  // T
    {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},  // S
    {{1, 0}, {0, 0}, {0, 1}, {-1, 1}},  // Z
    {{0, -1}, {0, 0}, {0, 1}, {1, 1}},  // L
    {{0, -1}, {0, 0}, {0, 1}, {-1, 1}}, // J
    {{0, -1}, {0, 0}, {0, 1}, {0, 2}}   // I
};

static_assert(
    Figure::INIT == 0 &&
    Figure::CLOCKWISE == 1 &&
    Figure::FLIP == 2 &&
    Figure::COUNTERCLOCK == 3, "You ruined hotspot array");

const Util::Point Figure::hotspot[NUM_TYPES][NUM_ROTATIONS] = {
    {{0, 0}, {0, 1}, {1, 1}, {1, 0}},   // O
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},   // T
    {{0, 0}, {0, 0}, {0, 1}, {-1, 0}},  // S
    {{0, 0}, {0, 0}, {0, 1}, {-1, 1}},  // Z
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},   // L
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},   // J
    {{0, 0}, {0, 0}, {0, 1}, {-1, 0}}   // I
};

Figure Map::Spawn()
{
    static std::uniform_int_distribution<> type_dist(0, Figure::NUM_TYPES - 1);
    static std::uniform_int_distribution<> rotation_dist(0, Figure::NUM_ROTATIONS - 1);

    const auto type = static_cast<Figure::Type>(type_dist(gen));
    const auto rotation = static_cast<Figure::Rotation>(rotation_dist(gen));
    const Util::Point position {static_cast<int>(grid.width() / 2), 0};

    return Figure(position, type, rotation);
}

void Map::Tick(Input in)
{
    std::cout << "Tick ";
    switch(state) {
    case State::INIT:
        std::cout << "init";
        figure = Spawn();
        state = DoesFigureCollide() ? State::END : State::FIGURE;
        break;
    case State::FIGURE:
        std::cout << "figure";
        MoveFigure(in);
        break;
    default:
    case State::END:
        std::cout << "end";
        break;
    }
    std::cout << '\n';
}

void Map::MoveFigure(Input in)
{
    const auto temp = figure;

    switch(in) {
    case Input::NIL:
    case Input::DOWN:
        figure.MoveDown();
        break;
    case Input::RIGHT:
        figure.MoveRight();
        break;
    case Input::LEFT:
        figure.MoveLeft();
        break;
    case Input::ROTATE:
        figure.RotateClockwise();
        break;
    }

    if(!IsFigureValid()) {
        figure = temp;
        switch (in) {
        case Input::NIL:
        case Input::DOWN:
            ToInitState();
            break;
        default:
            break;
        }
    }
}

bool Map::IsFigureValid()
{
    for(std::size_t i = 0; i < Figure::tetra; i++) {
        const auto p = figure[i];
        if(IsPointOutside(p) || DoesPointCollide(p)) {
            return false;
        }
    }
    return true;
}

bool Map::DoesFigureCollide()
{
    for(std::size_t i = 0; i < Figure::tetra; i++) {
        const auto p = figure[i];
        if(DoesPointCollide(p)) {
            return true;
        }
    }
    return false;
}

bool Map::IsPointOutside(const Util::Point& p)
{
    return
        p.x < 0 || p.x >= static_cast<int>(grid.width()) ||
        p.y < 0 || p.y >= static_cast<int>(grid.height());
}

bool Map::DoesPointCollide(const Util::Point& p)
{
    if(IsPointOutside(p)) {
        return false;
    } else {
        return grid[p] != 0;
    }
}

void Map::ToInitState()
{
    for(std::size_t i = 0; i < Figure::tetra; i++) {
        const auto p = figure[i];
        grid[p] = 1;
    }
    state = State::INIT;
}

} // namespace Tetris
