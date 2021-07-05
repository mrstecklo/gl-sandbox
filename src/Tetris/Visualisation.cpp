#include "Visualisation.h"
#include <iostream>

namespace Tetris {

glm::vec3 Visualisation::mapBase(0.f, -5.f, -10.f);
float Visualisation::step = 2.5f;

Visualisation::Visualisation(std::size_t width, std::size_t height, std::chrono::microseconds tickDuration, ResourceManager& res) :
    Map(width, height),
    resources(res),
    models(width, height),
    tickDur(tickDuration)
{}

static bool print = false;

void Visualisation::Proceed(std::chrono::microseconds delta, Map::Input in)
{
    counter += delta;
    if(counter >= tickDur || in != Map::Input::NIL) {
        Tick(in);
        print = true;
    }
    counter %= tickDur;
}

void Visualisation::ForEachModel(std::function<void(const GUI::Model&)> cb) const
{
    if(print) std::cout << "Visualisation::ForEachModel";
    std::size_t y = 0;
    for(auto&& row : models) {
        if(print) std::cout << "\n[" << y << "]:";
        std::size_t x = 0;
        for(auto&& model : row) {
            if(model.Valid()) {
                cb(model);
                if(print) std::cout << "1,";
            } else {
                if(print) std::cout << "0,";
            }
            ++x;
        }
        if(print) std::cout << ":(" << x << ")"; 
        ++y;
    }
    print = false;
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
    std::cout << "Visualisation::InitFigureImpl";
    for(auto&& p : GetFigure()) {
        std::cout << ' ' << p;
        Set(p, cube);
    }
    std::cout << '\n';
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

void Visualisation::FallCellsImpl(std::size_t first, std::size_t height, std::size_t destination)
{
    for(auto src = models.begin() + first; src < models.begin() + first + height; ++src, ++destination) {
        for(std::size_t x = 0; x < models.width(); ++x) {
            const Util::Point p {static_cast<int>(x), static_cast<int>(destination)};
            Set(p, std::move((*src)[x]));
        }
    }
}

} // namespace Tetris
