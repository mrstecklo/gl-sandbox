#pragma once

#include "Map.h"
#include "ResourceManager.h"
#include "GUI/Model.h"
#include <chrono>
#include <functional>

namespace Tetris {

class Visualisation {
public:

    Visualisation(std::size_t width, std::size_t height, std::chrono::microseconds tickDuration, ResourceManager& res);

    void Proceed(std::chrono::microseconds delta, Map::Input in);
    void ForEachModel(std::function<void(const GUI::Model&)> cb) const;

private:

    ResourceManager             resources;
    Map                         map;
    Util::Grid<GUI::Model>      models;
    std::chrono::microseconds   tick;
    std::chrono::microseconds   counter {0};
};

} // namespace Tetris