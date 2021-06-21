#pragma once

#include "Map.h"
#include "ResourceManager.h"
#include "GUI/Model.h"
#include <chrono>
#include <functional>

namespace Tetris {

class Visualisation : public Map {
public:

    Visualisation(std::size_t width, std::size_t height, std::chrono::microseconds tickDuration, ResourceManager& res);

    void Proceed(std::chrono::microseconds delta, Map::Input in);
    void ForEachModel(std::function<void(const GUI::Model&)> cb) const;

protected:

    void InitFigureImpl() override;
    void SolidifyImpl() override;
    void MoveFigureImpl(Input in, const Figure& old) override;
    void MoveRestictedImpl() override;
    void CleanRowImpl(std::size_t idx) override;
    void FallCellsImpl(std::size_t column, std::size_t first, std::size_t height, std::size_t destination) override;

private:

    static glm::vec3 Position(const Util::Point& p) { return mapBase + glm::vec3(p.x * step, p.y * step, -5.f); }
    void Set(const Util::Point& p, const GUI::Model& m);
    void Set(const Util::Point& p, GUI::Model&& m);

    ResourceManager&            resources;
    Util::Grid<GUI::Model>      models;
    std::chrono::microseconds   tickDur;
    std::chrono::microseconds   counter {0};

    static glm::vec3 mapBase;
    static float step;
};

} // namespace Tetris
