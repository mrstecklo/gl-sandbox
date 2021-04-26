#include "Object.h"

namespace GUI {

void Object::LookAt(const glm::vec3& eye, const glm::vec3& up)
{
    rotation = LookInDirection(eye - position, up);
}

void Object::LookAt(const glm::vec3& pos, const glm::vec3& eye, const glm::vec3& up)
{
    position = pos;
    LookAt(eye, up);
}

glm::quat Object::LookInDirection(const glm::vec3& d, const glm::vec3& up)
{
    // assuming that initial direction is towards -Z, i.e. glm::vec3 orig(0, 0, -1) 
    // and initial 'up' direction is towards +Y, i.e.      glm::vec3 origUp(0, 1, 0)
    // this makes computations much more efficient

    const auto distance2 = glm::length2(d);

    if(!std::isnormal(distance2)) {
        return glm::quat();
    }

    const float distance = std::sqrt(distance2);    // this actually must be length(d) * length(orig)
    const auto w = distance - d.z;                  // (1 + cos(alpha)) * distance   

    if(w > 0.0001f * distance) {    // cos(alpha) != -1, alpha != pi
        // glm::quat first(w, -d.y, d.x, 0.f)
        // newUp is inv(first) * origUp - position of Y after first rotation
        // vectors and quaternions are not normalized until return, this allows to simplify computations
        const glm::vec3 newUp( // norm(newUp) = distance * w
            -d.x * d.y,
            d.x * d.x - d.z * w,
            w * d.y
        );

        // this is a projection of 'up' on a plane orthogonal to 'd'
        // it is also multiplied by distance2 to avoid division
        const auto corrUp = distance2 * up - d * glm::dot(up, d);

        // these threee represent angle between corrUp and newUp, beta
        const auto a = distance * glm::dot(up, newUp);          // c * cos(beta), [glm::dot(up, newUp) == glm::dot(corrUp, newUp) / distance2]
        const auto b = glm::length(glm::cross(newUp, corrUp));  // c * sin(beta) * distance
        const auto c = glm::length(corrUp) * w;                 // hypothenuse

        const auto den = c - a;                                 // c * (1 - cos(beta))

        if(std::isnormal(c) && (den > c * 0.0001f)) { // up is not collinear with d and (cos(beta) != 1, beta != 0)
            const auto dd = den * distance;
            // glm::quat second(b, d.x *dd, d.y * dd, d.z * dd)
            return glm::normalize(glm::quat( // second * first
                w * b,
                dd * d.x - d.y * b,
                dd * d.y + d.x * b,
                dd * d.z - den * distance2
            ));
        } else {                                      // up is collinear with d or (cos(beta) == 1, beta == 0)
            // glm::quat second(1, 0, 0, 0)
            return glm::normalize(glm::quat(w, -d.y, d.x, 0.f)); // second * first
        }
    } else { // cos(alpha) == -1, alpha == pi, d = glm::vec3(0, 0, distance)
        // glm::quat first(0, 0, 1, 0) - rotate around Y
        // glm::vec3 newUp(0, 1, 0) == origUp
        // glm::vec3 corrUp(up.x, up.y, 0)
        const auto c = std::sqrt(up.y * up.y + up.x * up.x);
        const auto den = c - up.y;                              // c * (1 - cos(beta))

        if(std::isnormal(c) && (den > c * 0.0001f)) { // up is not collinear with d and (cos(beta) != 1, beta != 0)
            // glm::quat second(up.x, 0, 0, den)
            return glm::normalize(glm::quat(0.f, den, up.x, 0.f)); // second * first
        } else {                                      // up is collinear with d or (cos(beta) == 1, beta == 0)
            // glm::quat second(1, 0, 0, 0)
            return glm::quat(0.f, 0.f, 1.f, 0.f); // second * first
        }
    }
}

} // namespace GUI
