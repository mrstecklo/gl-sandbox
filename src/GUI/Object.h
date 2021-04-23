#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace GUI {

class Object {
public:
    Object() = default;
    Object(const glm::vec3& p, const glm::quat& r) :
        position(p),
        rotation(r)
    {}

    void SetPosition(const glm::vec3& p) { position = p; }
    void SetRotation(const glm::quat& r) { rotation = r; }

    void LookAt(const glm::vec3& pos, const glm::vec3& up);

    const glm::vec3& GetPosition() const { return position; }
    const glm::quat& GetRotation() const { return rotation; }


    static constexpr float sqr(float x) { return x * x; }

private:
    glm::vec3 position;
    glm::quat rotation;
};

} // namespace GUI
