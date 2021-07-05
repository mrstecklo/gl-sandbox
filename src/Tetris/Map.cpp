#include "Map.h"

#include <iostream>
#include <algorithm>

namespace Tetris {

#ifndef FULL_CONSTEXPR
CONSTEXPR14 Util::Point FigureConstIterator::operator*() { return (*parent)[idx]; }
#endif

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

Map::Map(std::size_t width, std::size_t height) :
    grid(width, height),
    gen(std::random_device()())
{
    if(width > static_cast<std::size_t>(std::numeric_limits<int>::max()) || height > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::runtime_error("Map::Map. Dimensions are too big");
    }
}

Figure Map::Spawn()
{
    static std::uniform_int_distribution<> type_dist(0, Figure::NUM_TYPES - 1);
    static std::uniform_int_distribution<> rotation_dist(0, Figure::NUM_ROTATIONS - 1);

    const auto type = static_cast<Figure::Type>(type_dist(gen));
    const auto rotation = static_cast<Figure::Rotation>(rotation_dist(gen));
    const Util::Point position {static_cast<int>(grid.width() / 2), static_cast<int>(grid.height())};

    std::cout << '\n' << "Map::Spawn " << type << ", " << rotation << ", " <<  position;

    return Figure(position, type, rotation);
}

void Map::Tick(Input in)
{
    std::cout << "Tick ";
    switch(state) {
    case State::INIT:
        std::cout << "init";
        figure = Spawn();
        while(IsFigureOutside()) {
            figure.MoveDown();
        }
        state = DoesFigureCollide() ? State::END : State::FIGURE;
        InitFigureImpl();
        break;
    case State::FIGURE:
        std::cout << "figure";
        std::cout << " " << figure[0];
        MoveFigure(in);
        break;
    case State::CLEAN:
        std::cout << "clean";
        Clean();
        break;
    case State::GRAVITY:
        std::cout << "gravity";
        Fall();
        break;
    case State::END:
        std::cout << "end";
        break;
    }
    std::cout << '\n';
}

void Map::MoveFigure(Input in)
{
    const auto old = figure;

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
    case Input::CLOCKWISE:
        figure.RotateClockwise();
        break;
    case Input::COUNTERCLOCK:
        figure.RotateCounterClockwise();
        break;
    }

    if(IsFigureValid()) {
        MoveFigureImpl(in, old);
    } else {
        figure = old;
        switch (in) {
        case Input::NIL:
        case Input::DOWN:
            Solidify();
            state = State::CLEAN;
            break;
        default:
            MoveRestictedImpl();
            break;
        }
    }
}

bool Map::IsFigureValid() const
{
    for(const auto&& p : figure) {
        if(IsPointOutside(p) || DoesPointCollide(p)) {
            return false;
        }
    }
    return true;
}

bool Map::DoesFigureCollide() const
{
    for(const auto&& p : figure) {
        if(DoesPointCollide(p)) {
            return true;
        }
    }
    return false;
}

bool Map::IsFigureOutside() const
{
    for(const auto&& p : figure) {
        if(IsPointOutside(p)) {
            return true;
        }
    }
    return false;
}

bool Map::IsPointOutside(const Util::Point& p) const
{
    return
        p.x < 0 || p.x >= static_cast<int>(grid.width()) ||
        p.y < 0 || p.y >= static_cast<int>(grid.height());
}

bool Map::DoesPointCollide(const Util::Point& p) const
{
    if(IsPointOutside(p)) {
        return false;
    } else {
        return grid[p] != Cell::EMPTY;
    }
}

void Map::Solidify()
{
    for(const auto&& p : figure) {
        grid[p] = Cell::SOLID;
    }
    SolidifyImpl();
}

void Map::Clean()
{
    int indexes[Figure::tetra] = {-1, -1, -1, -1};
    static const auto alreadyChecked = [&indexes] (int idx) {
        for(auto& i : indexes) {
            if(i  < 0) {
                i = idx;
                return false;
            } else if(i == idx) {
                return true;
            } // else continue
        }
        throw std::runtime_error("Map::Clean invalid index");
    };

    for(const auto&& p : figure) {
        if(!alreadyChecked(p.y)) {
            if(IsRowFilled(p.y)) {
                CleanRow(p.y);
            }
        }
    }
    state = State::GRAVITY;
}

bool Map::IsRowFilled(std::size_t idx) const
{
    for(const auto c : grid[idx]) {
        if(c == Cell::EMPTY) {
            return false;
        }
    }
    return true;
}

void Map::CleanRow(std::size_t idx) {
    grid[idx][0] = Cell::DISAPPEARING;
    CleanRowImpl(idx);
}

void Map::Fall() {
    auto gravityState = GravityState::SEEK_DISAPPEARING;
    std::size_t firstDisappearing = 0;
    std::size_t firstUnchanged = 0;
    
    for(std::size_t y = 0; y < grid.height(); ++y) {
        switch (gravityState) {
        case GravityState::SEEK_DISAPPEARING:
            if(grid[y][0] == Cell::DISAPPEARING) {
                FallCells(firstUnchanged, y - firstUnchanged, firstDisappearing);
                firstDisappearing = y;
                gravityState = GravityState::SEEK_UNCHANGED;
            }
            break;
        case GravityState::SEEK_UNCHANGED:
            if(grid[y][0] != Cell::DISAPPEARING) {
                firstUnchanged = y;
                gravityState = GravityState::SEEK_DISAPPEARING;
            }
            break;
        }
    }
    FallCells(firstUnchanged, grid.height() - firstUnchanged, firstDisappearing);
    state = State::INIT;
}

void Map::FallCells(std::size_t first, std::size_t height, std::size_t destination)
{
    if(first > destination) {
        for(auto src = grid.begin() + first, dst = grid.begin() + destination; src < grid.begin() + first + height; ++src, ++dst) {
            std::memcpy(dst->data(), src->data(), grid.width() * sizeof(Cell));
            std::memset(dst->data(), 0, grid.width() * sizeof(Cell));
            static_assert(Cell::EMPTY == static_cast<Cell>(0), "memset '0' implies Cell::EMPTY is zero");
            static_assert(std::is_same<decltype(grid)::value_type, Cell>::value, "Grid here is not made of cells");
        }

        FallCellsImpl(first, height, destination);
    }
}

} // namespace Tetris
