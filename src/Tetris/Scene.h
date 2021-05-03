#include "GUI/Scene.h"
#include "GLCPP/VertexArray.h"
#include "ResourceManager.h"

namespace Tetris {

class Scene : public GUI::Scene {
public:
    Scene() = default;
    ~Scene() override = default;

private:
    void InitImpl() override;
    void ResizeImpl(int width, int height) override;
    void ProcessInputImpl(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame) override;

    const GUI::Camera& GetCameraImpl() const override { return camera; }
    void ForEachObjectImpl(ObjectCb cb) const override;
    void ForEachModelImpl(ModelCb cb) const override;

    GUI::Camera camera;
    float       mouseSpeed = 0.000005f;
    float       speed = 0.000005f;

    GL::VertexArray arr {nullptr};

    ResourceManager resources;

    GUI::Model cube;
    GUI::Model triangle;

    
};

} // namespace Tetris
