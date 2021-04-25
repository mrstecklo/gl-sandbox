#include "Object.h"

namespace GUI {



void Object::LookAt(const glm::vec3& pos, const glm::vec3& up)
{
    auto d = pos - position;
    auto distance2 = glm::length2(d);

    if(!std::isnormal(distance2)) {
        rotation = glm::quat();
        return;
    }

    const float distance = std::sqrt(distance2);
    const auto w = distance - d.z;

    if(w > 0.0001f * distance) {
        glm::quat first(w, -d.y, d.x, 0.f);

        glm::vec3 newUp(                    // norm(newUp) = distance * w
            -d.x * d.y,
            d.x * d.x - d.z * w,
            w * d.y
        );

        auto corrUp = distance2 * up - d * glm::dot(up, d);

        auto upProjection = glm::length(corrUp);

        const auto a = distance * glm::dot(up, newUp);
        const auto b = glm::length(glm::cross(newUp, corrUp));
        const auto c = upProjection * w;

        auto den = c - a;

        if(std::isnormal(c) && (den > c * 0.0001f)) {
            auto r = b / den;

            rotation = glm::normalize(glm::quat(
                w * r,
                distance * d.x  - d.y * r,
                distance * d.y  + d.x * r,
                - d.y * d.y     - newUp.y
            ));
        } else {
            rotation = glm::normalize(first);
        }
    } else {
        const auto c = std::sqrt(up.y * up.y + up.x * up.x);

        auto den = c - up.y;

        if(std::isnormal(c) && (den > c * 0.0001f)) {
            rotation = glm::normalize(glm::quat(0.f, 1.f, up.x / den, 0.f));
        } else {
            rotation = glm::quat(0.f, 0.f, 1.f, 0.f);
        }
    }
}

} // namespace GUI
