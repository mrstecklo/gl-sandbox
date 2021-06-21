#include "Visualisation.h"


namespace Tetris {

glm::vec3 Visualisation::mapBase(0.f, 0.f, -10.f);
float Visualisation::step = 2.5f;

Visualisation::Visualisation(std::size_t width, std::size_t height, std::chrono::microseconds tickDuration, ResourceManager& res) :
    Map(width, height),
    resources(res),
    models(width, height),
    tickDur(tickDuration)
{}

void Visualisation::Proceed(std::chrono::microseconds delta, Map::Input in)
{
    counter += delta;
    if(counter >= tickDur || in != Map::Input::NIL) {
        Tick(in);
    }
    counter %= tickDur;
}

void Visualisation::ForEachModel(std::function<void(const GUI::Model&)> cb) const
{
    for(auto&& row : models) {
        for(auto&& model : row) {
            if(model.Valid()) {
                cb(model);
            }
        }
    }
}

void Visualisation::Set(const Util::Point& p, const GUI::Model& m)
{
    auto& destination = models[p];
    destination = m;
    destination.SetPosition(Position(p));
}

void Visualisation::Set(const Util::Point& p, GUI::Model&& m)
{
    auto& destination = models[p];
    destination = std::move(m);
    destination.SetPosition(Position(p));
}

void Visualisation::InitFigureImpl()
{
    const auto cube = resources.CreateModel(Resource::V_CUBE, Resource::T_AXIS, Resource::P_GENERIC);
    for(auto&& p : GetFigure()) {
        Set(p, cube);
    }
}

void Visualisation::SolidifyImpl()
{
    const auto cube = resources.CreateModel(Resource::V_CUBE, Resource::T_TEMPLATE, Resource::P_GENERIC);
    for(auto&& p : GetFigure()) {
        Set(p, cube);
    }
}

void Visualisation::MoveFigureImpl(Input /* in */, const Figure& old)
{
    for(std::size_t i = 0; i < Figure::tetra; ++i) {
        Set(GetFigure()[i], std::move(models[old[i]]));
    }
}

void Visualisation::MoveRestictedImpl()
{

}

void Visualisation::CleanRowImpl(std::size_t idx)
{
    for(auto& model : models[idx]) {
        model = GUI::Model();
    }
}

void Visualisation::FallCellsImpl(std::size_t column, std::size_t first, std::size_t height, std::size_t destination)
{
    const auto last = first + height;

    for(auto src = models.begin() + first; src < models.begin() + last; ++src, ++destination) {
        Set({static_cast<int>(column), static_cast<int>(destination)}, std::move((*src)[column]));
    }
}

} // namespace Tetris
