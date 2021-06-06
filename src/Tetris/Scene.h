#include "GUI/Scene.h"
#include "GLCPP/VertexArray.h"
#include "ResourceManager.h"
#include "Map.h"

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

    void ForEachModelImpl(ModelCb cb) override;

    GUI::Camera camera;
    float       mouseSpeed = 0.00000005f;
    float       speed = 0.000005f;

    GL::VertexArray arr {nullptr};

    ResourceManager resources;

    GUI::Model scube;
    GUI::Model cube;
    GUI::Model triangle;

    GUI::Model lookingCube;
    GUI::Model FPLookingCube;

    Map        map;

    std::chrono::microseconds counter {0};

    static constexpr std::size_t width = 10;
    static constexpr std::size_t height = 20;
    static constexpr std::chrono::microseconds tickDuration { 500000 };
};

} // namespace Tetris
