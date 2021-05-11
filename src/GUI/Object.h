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

    void Translate(const glm::vec3& p) { position += p; }

    void Rotate(const glm::quat& r) { rotation = glm::normalize(rotation * r); }
    // rotate relative to model space
    void MSRotate(const glm::quat& r) { rotation = glm::normalize(r * rotation); }

    // rotates object keeping its Y as close to global Y as possible (just like first-player camera)
    // resulting nutation is clamped to [-pi / 2; pi / 2]
    void FPRotate(float precession, float nutation);

    // sets rotation = LookInDirection(eye - position, up)
    void LookAt(const glm::vec3& eye, const glm::vec3& up);
    // sets position to pos, then calls LookAt(eye, up)
    void LookAt(const glm::vec3& pos, const glm::vec3& eye, const glm::vec3& up);

    // sets rotation = FPLookInDirection(eye - position)
    void FPLookAt(const glm::vec3& eye);
    // sets position to pos, then calls FPLookAt(eye)
    void FPLookAt(const glm::vec3& pos, const glm::vec3& eye);

    const glm::vec3& GetPosition() const { return position; }
    const glm::quat& GetRotation() const { return rotation; }

    // returns objects -Z direction
    glm::vec3 GetDirection() const;
    // returns objects +X direction
    glm::vec3 GetRight() const;
    // returns objects +Y direction
    glm::vec3 GetUp() const;

    // return quaternion that turns object's -Z to dir and Y to cross(dir, cross(up, dir))
    static glm::quat LookInDirection(const glm::vec3& dir, const glm::vec3& up);
    // the same as LookInDirection, but up is glm::vec3(0, 1, 0)
    // this implementation is more efficient
    static glm::quat FPLookInDirection(const glm::vec3& dir);

    static constexpr float sqr(float x) { return x * x; }

private:

    static glm::quat FPRotateImpl(float cosAlpha, float sinAlpha, float cosBeta, float sinBeta);

    glm::vec3 position;
    glm::quat rotation;
};

} // namespace GUI
