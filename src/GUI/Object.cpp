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

void Object::FPLookAt(const glm::vec3& eye)
{
    rotation = FPLookInDirection(eye - position);   
}

void Object::FPLookAt(const glm::vec3& pos, const glm::vec3& eye)
{
    position = pos;
    FPLookAt(eye);
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

	// first rotate around cross(dir, orig)
    const float distance = std::sqrt(distance2);    // this actually must be length(d) * length(orig)
    const auto w = distance - d.z;                  // (1 + cos(alpha)) * distance   

    if(w > 0.00001f * distance) {    // cos(alpha) != -1, alpha != pi
        // glm::quat first(w, -d.y, d.x, 0.f)
        // newUp is inv(first) * origUp - position of Y after first rotation
        // vectors and quaternions are not normalized until return, this allows to simplify computations
        const glm::vec3 newUp( // norm(newUp) = distance * w
            -d.x * d.y,
            sqr(d.x) - d.z * w,
            w * d.y
        );

		// now rotate around dir, so that object's Y is up
		
        // this is a projection of 'up' on a plane orthogonal to 'd'
        // it is also multiplied by distance2 to avoid division
        const auto corrUp = distance2 * up - d * glm::dot(up, d);

        // these threee represent angle between corrUp and newUp, beta
        const auto a = glm::dot(corrUp, newUp);                 // c * cos(beta), [glm::dot(up, newUp) == glm::dot(corrUp, newUp) / distance2]
        const auto b = glm::length(glm::cross(newUp, corrUp));  // c * sin(beta)
        const auto c = distance * w * glm::length(corrUp);      // hypothenuse

        const auto den = c - a;                                 // c * (1 - cos(beta))

        if(std::isnormal(c) && (den > c * 0.00001f)) { // up is not collinear with d and (cos(beta) != 1, beta != 0)
            // glm::quat second(b, d.x *den, d.y * den, d.z * den)
            return glm::normalize(glm::quat( // second * first
                w * b,
                den * d.x - d.y * b,
                den * d.y + d.x * b,
                -den * w
            ));
        } else {                                      // up is collinear with d or (cos(beta) == 1, beta == 0)
            // glm::quat second(1, 0, 0, 0)
            return glm::normalize(glm::quat(w, -d.y, d.x, 0.f)); // second * first
        }
    } else { // cos(alpha) == -1, alpha == pi, d = glm::vec3(0, 0, distance)
        // glm::quat first(0, 0, 1, 0) - rotate around Y
        // glm::vec3 newUp(0, 1, 0) == origUp
        // glm::vec3 corrUp(up.x, up.y, 0)
        const auto c = std::sqrt(sqr(up.y) + sqr(up.x));
        const auto den = c - up.y;                              // c * (1 - cos(beta))

        if(std::isnormal(c) && (den > c * 0.00001f)) { // up is not collinear with d and (cos(beta) != 1, beta != 0)
            // glm::quat second(up.x, 0, 0, den)
            return glm::normalize(glm::quat(0.f, den, up.x, 0.f)); // second * first
        } else {                                      // up is collinear with d or (cos(beta) == 1, beta == 0)
            // glm::quat second(1, 0, 0, 0)
            return glm::quat(0.f, 0.f, 1.f, 0.f); // second * first
        }
    }
}

glm::quat Object::FPLookInDirection(const glm::vec3& d)
{
    // the same as LookInDirection, but up is glm::vec3(0, 1, 0)
    const float distance = glm::length(d);

    if(!std::isnormal(distance)) {
        return glm::quat();
    }

    // first rotate around Y - alpha
    // then rotate around object's X - beta

    const auto horiz = std::sqrt(sqr(d.x) + sqr(d.z));
    const auto w1 = distance + horiz; // distance * (1 + cos(alpha))

    if(std::isnormal(horiz)) {
        const auto w2 = horiz - d.z; // horiz * (1 + cos(beta))

        if (w2 > horiz * 0.00001f) {
            return glm::normalize(glm::quat( w1 * w2, -d.y * w2, w1 * d.x, -d.y * d.x));
        } else {
            return glm::normalize(glm::quat( 0.f, 0.f, w1, -d.y ));
        }
    } else {
        return glm::normalize(glm::quat( w1, -d.y, 0.f, 0.f ));
    }
}

void Object::FPRotate(float precession, float nutation)
{
    const auto cosAlpha = sqr(rotation.w) + sqr(rotation.y) - sqr(rotation.x) - sqr(rotation.z);
    const auto sinAlpha = 2.f * (rotation.w * rotation.x + rotation.y * rotation.z);

    const auto cosBeta = sqr(rotation.w) + sqr(rotation.x) - sqr(rotation.y) - sqr(rotation.z);
    const auto sinBeta = 2.f * (rotation.w * rotation.y + rotation.x * rotation.z);

    const auto sinP = std::sin(precession);
    const auto cosP = std::cos(precession);

    const auto sinN = std::sin(nutation);
    const auto cosN = std::cos(nutation);

    rotation = FPRotateImpl(
        cosAlpha * cosN - sinAlpha * sinN,
        sinAlpha * cosN + cosAlpha * sinN,
        cosBeta  * cosP - sinBeta  * sinP,
        sinBeta  * cosP + cosBeta  * sinP
    );
}

glm::quat Object::FPRotateImpl(float cosAlpha, float sinAlpha, float cosBeta, float sinBeta)
{
    const auto w2 = 1 + cosBeta;

    if(cosAlpha <= 0.f) { // clamp nutation to [-pi / 2; pi / 2]
        if(sinAlpha > 0) {
            if(w2 > 0.00001f) {
                return glm::normalize(glm::quat( w2, w2, sinBeta, sinBeta ));
            } else {
                return glm::normalize(glm::quat( 0.f, 0.f, 1.f, 1.f ));
            }
        } else {
            if(w2 > 0.00001f) {
                return glm::normalize(glm::quat( w2, -w2, sinBeta, -sinBeta ));
            } else {
                return glm::normalize(glm::quat( 0.f, 0.f, 1.f, -1.f ));
            }
        }
    } else {
        const auto w1 = 1 + cosAlpha;
        if(w2 > 0.00001f) {
            return glm::normalize(glm::quat( w1 * w2, sinAlpha * w2, w1 * sinBeta, sinAlpha * sinBeta ));
        } else {
            return glm::normalize(glm::quat( 0.f, 0.f, w1, sinAlpha ));
        }
    }
}

glm::vec3 Object::GetDirection() const
{
    return glm::vec3(
         2.f * (rotation.w * rotation.y - rotation.z * rotation.x),
        -2.f * (rotation.w * rotation.x + rotation.z * rotation.y),
         2.f * (rotation.x * rotation.x + rotation.y * rotation.y) - 1.f
    );
}

} // namespace GUI
