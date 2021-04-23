#include "Object.h"

namespace GUI {



void Object::LookAt(const glm::vec3& pos, const glm::vec3& up)
{
    const auto orig = glm::vec3(0.f, 0.f, -1.f);

    auto d = pos - position;

    if(glm::length2(d) < 0.0001f) {
        rotation = glm::quat();
        return;
    }

    auto dir = glm::normalize(d);

    auto cos1 = glm::dot(orig, dir);

    auto first = (glm::dot(dir, orig) < -1.f + 0.001f)
        ? glm::quat(0.f, 0.f, 1.f, 0.f)
        : glm::quat(dir, orig);

    auto theta = 2.f * std::acos(first.w);
    auto pitheta = theta / 3.1415926535897932f;

    auto cos2 = std::cos(theta);

    const auto sqrSin = sqr(first.x) + sqr(first.y) + sqr(first.z);
    auto newUp = glm::normalize(glm::vec3(0.f, sqr(first.w) - sqrSin, 0.f) + 2.f * (first.w * glm::vec3(-first.z, 0.f, -first.x) + first.y * glm::vec3(first.x, first.y, first.z)));
    //auto newUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * first);

    auto corrUp = glm::normalize(glm::cross(glm::cross(dir, up), dir));

    auto cos = glm::dot(newUp, corrUp);

    auto second = (cos < -1.f + 0.001f)
        ? glm::quat(0.f, dir)
        : glm::quat( // TODO: there must be a simpler way as we are always rotating around dir
            corrUp,
            newUp
        );

    rotation = first * second;
}

} // namespace GUI
