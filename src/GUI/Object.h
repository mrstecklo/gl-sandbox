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
    void SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }
    void SetRotation(const glm::quat& r) { rotation = r; }
    void SetRotation(float w, float x, float y, float z) { SetRotation(glm::quat(w, x, y, z)); }

    void Translate(const glm::vec3& p) { position += p; }
    void Translate(float x, float y, float z) { Translate(glm::vec3(x, y, z)); }

    void Rotate(const glm::quat& r) { rotation = glm::normalize(r * rotation); }
    void Rotate(float w, float x, float y, float z) { Rotate(glm::quat(w, x, y, z)); }
    // rotate relative to model space
    void MSRotate(const glm::quat& r) { rotation = glm::normalize(rotation * r); }
    void MSRotate(float w, float x, float y, float z) { MSRotate(glm::quat(w, x, y, z)); }

    // rotates object keeping its Y as close to global Y as possible (just like first-player camera)
    // resulting nutation is clamped to [-pi / 2; pi / 2]
    void FPRotate(float precession, float nutation);
    static glm::quat FPRotate(const glm::quat& init, float precession, float nutation);

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

    // returns object's -Z direction
    glm::vec3 GetDirection() const { return GetDirection(rotation); }
    // returns object's +X direction
    glm::vec3 GetRight() const { return GetRight(rotation); }
    // returns object's +Y direction
    glm::vec3 GetUp() const { return GetUp(rotation); }

    // returns r * {0, 0, -1}
    static glm::vec3 GetDirection(const glm::quat& r);
    // returns r * {1, 0, 0}
    static glm::vec3 GetRight(const glm::quat& r);
    // returns r * {0, 1, 0}
    static glm::vec3 GetUp(const glm::quat& r);

    // return quaternion that turns object's -Z to dir and Y to cross(dir, cross(up, dir))
    static glm::quat LookInDirection(const glm::vec3& dir, const glm::vec3& up);
    // the same as LookInDirection, but up is glm::vec3(0, 1, 0)
    // this implementation is more efficient
    static glm::quat FPLookInDirection(const glm::vec3& dir);

    static constexpr float sqr(float x) { return x * x; }

private:

    static glm::quat FPRotateImpl(float cosAlpha, float sinAlpha, float cosBeta, float sinBeta);

    glm::vec3 position{0.f, 0.f, 0.f};
    glm::quat rotation{1.f, 0.f, 0.f, 0.f};
};

} // namespace GUI
