#include "Visualisation.h"


namespace Tetris {

Visualisation::Visualisation(std::size_t width, std::size_t height, std::chrono::microseconds tickDuration, ResourceManager& res) :
    resources(res),
    map(width, height),
    models(width, height, resources.CreateModel(Resource::V_CUBE, Resource::T_AXIS, Resource::P_GENERIC)),
    tick(tickDuration)
{}

void Visualisation::Proceed(std::chrono::microseconds delta, Map::Input in)
{
    counter += delta;
    if(counter >= tick || in != Map::Input::NIL) {
        map.Tick(in);
    }
    counter %= tick;

    auto& grid = map.GetGrid();

    static glm::vec3 mapBase(0.f, 0.f, -10.f);
    static float step = 2.5f;
    
    for(std::size_t j = 0; j < grid.height(); ++j) {
        auto row = grid[j];
        for(std::size_t i = 0; i < grid.width(); ++i) {
            if(row[i] != 0) {
                const auto pos = mapBase + glm::vec3(step * i, step * j, 0.f);
                cube.SetPosition(pos);
                cb(cube);
            }
        }
    }
}

void Visualisation::ForEachModel(std::function<void(const GUI::Model&)> cb) const
{
    for(auto&& row : models) {
        for(auto&& model : row) {
            cb(model);
        }
    }
}

} // namespace Tetris