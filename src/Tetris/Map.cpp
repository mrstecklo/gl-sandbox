#include "Map.h"


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
}

} // namespace Tetris
