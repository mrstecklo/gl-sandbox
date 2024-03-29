#include "GUI/Scene.h"
#include "GLCPP/VertexArray.h"
#include "ResourceManager.h"
#include "Visualisation.h"

namespace Tetris {

class Scene : public GUI::Scene {
public:
    Scene();
    ~Scene() override = default;

private:
    void InitImpl() override;
    void ResizeImpl(int width, int height) override;
    void ProcessInputImpl(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame) override;

    const GUI::Camera& GetCameraImpl() const override { return camera; }

    void ForEachModelImpl(ModelCb cb) const override;

    GUI::Camera camera;
    float       mouseSpeed = 0.00000005f;
    float       speed = 0.000005f;

    GL::VertexArray arr {nullptr};

    ResourceManager resources;

    GUI::Model scube;
    mutable GUI::Model cube;
    GUI::Model triangle;

    GUI::Model lookingCube;
    GUI::Model FPLookingCube;

    Visualisation map;

    std::chrono::microseconds counter {0};

    static constexpr std::size_t mapWidth = 10;
    static constexpr std::size_t mapHeight = 20;
};

} // namespace Tetris
