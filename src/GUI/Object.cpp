#include "Object.h"

namespace GUI {



void Object::LookAt(const glm::vec3& pos, const glm::vec3& up)
{
    //const auto orig = glm::vec3(0.f, 0.f, -1.f);

    auto d = pos - position;

    auto distance2 = glm::length2(d);

    if(!std::isnormal(distance2)) {
        rotation = glm::quat();
        return;
    }

    

    const float distance = std::sqrt(distance2); // norm(orig) == 1.f
    const auto cosAlpha = -d.z / distance; // dot(d, orig) / distance
    //const auto halfSinAlpha2 = 0.5f * (1.f - cosAlpha);
    const auto halfCosAlpha2 = 0.5f * (1.f + cosAlpha);
    const auto halfCosAlpha = std::sqrt(halfCosAlpha2);

    const auto axisNorm = distance * 2.f * halfCosAlpha;

    /*glm::vec3 axis = std::isnormal(axisNorm)
         ? glm::vec3(-d.y / axisNorm, d.x / axisNorm, 0.f) // cross(d, orig)
         : glm::vec3(0.f, 1.f, 0.f);*/

    const auto w = distance - d.z;

    auto first = (w > 0.0001 * distance)
        ? glm::quat(w, -d.y, d.x, 0.f)
        : glm::quat(0.f, 0.f, 1.f, 0.f);

    /* glm::vec3 newUp(
        2.f * (axis.y * axis.x),
        2.f * (axis.y * axis.y) + cosAlpha,
        2.f * (-halfCosAlpha * axis.x)
    ); */

    glm::vec3 newUp(
        -2.f * (d.x * d.y),
         2.f * (d.x * d.x - d.z * w),
         2.f * (w * d.y)
    );

    //auto corrUp = glm::cross(glm::cross(d, up), d);
    auto corrUp = distance2 * up - d * glm::dot(up, d);
    auto upProjection2 = glm::length2(corrUp);

/*     if(!std::isnormal(upProjection2)) {
        //rotation = glm::normalize(glm::quat(halfCosAlpha, axis.x, axis.y, 0.f));
        rotation = glm::normalize(first);
        return;
    } */

    float upProjection = std::sqrt(upProjection2);
    float cosBeta = (std::isnormal(upProjection2))
        ? glm::dot(up, newUp) * (0.5f / (upProjection *  (1.f + cosAlpha)))
        : 1.f;
    float halfSinBeta = std::sqrt(0.5f * (1.f - cosBeta));
    float halfCosBeta = std::sqrt(0.5f * (1.f + cosBeta));

    const auto axis2norm = upProjection * 2.f * halfCosBeta;

    auto axis2 = d * (halfSinBeta / distance);
    //glm::quat second(halfCosBeta, axis2.x, axis2.y, axis2.z);

    rotation = glm::normalize(glm::quat(
        first.w * halfCosBeta   - first.x * axis2.x         - first.y * axis2.y,
        first.w * axis2.x       + first.x * halfCosBeta     + first.y * axis2.z,
        first.w * axis2.y       + first.y * halfCosBeta     - first.x * axis2.z,
        first.w * axis2.z       + first.x * axis2.y         - first.y * axis2.x
    ));

    //rotation = glm::normalize(first * second);
    //rotation = first;
}

} // namespace GUI
