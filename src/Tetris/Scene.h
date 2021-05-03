#include "GUI/Scene.h"
#include "GLCPP/VertexArray.h"

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
    

    static constexpr GLint vertexSize = 3;
    static constexpr GLint UVSize = 2;
    static constexpr GLsizei numVertices = 36;

    static constexpr auto attrVertices = GL::VertexAttrib(0);
    static constexpr auto attrUV = GL::VertexAttrib(1);

    static const GLchar vertexShader[];
    static const GLchar fragmentShader[];

    static const GLfloat g_vertex_buffer_data[numVertices][vertexSize];
    static const GLfloat g_uv_buffer_data[numVertices][UVSize];

    GUI::Camera camera;

    GL::VertexArray arr {nullptr};

    GUI::Program program;

    GUI::Model cube;
    GUI::Model triangle;

    float               mouseSpeed = 0.000005f;
    float               speed = 0.000005f;
};

} // namespace Tetris
