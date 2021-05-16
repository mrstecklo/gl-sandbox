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
    const float distance = std::sqrt(distance2);                        // this actually must be length(d) * length(orig)
    const auto w = distance - d.z;                                      // (1 + cos(alpha)) * distance   

    if(w > 0.00001f * distance) {                                       // cos(alpha) != -1, alpha != pi
        const glm::quat first(w, d.y, -d.x, 0.f);
        // newUp is first * origUp - position of Y after first rotation
        // vectors and quaternions are not normalized until return, this allows to simplify computations

        const glm::vec3 newUp = GetUp(first);
        const auto normNewUp = 2.f * distance * w;                      // glm::length(newUp)

		// now rotate around dir, so that object's Y is up
		
        // this is a projection of 'up' on a plane orthogonal to 'd'
        // it is also multiplied by distance2 to avoid division
        const auto corrUp = distance2 * up - d * glm::dot(up, d);

        const auto c = normNewUp * glm::length(corrUp);                 // hypothenuse

        if(std::isnormal(c)) {                                          // up is not collinear with d
            const auto a = glm::dot(corrUp, newUp);                     // c * cos(beta), [glm::dot(up, newUp) == glm::dot(corrUp, newUp) / distance2]
            const auto den = c + a;                                     // c * (1 + cos(beta))
            if(den > c * 0.00001f) {                                    // cos(beta) != -1, beta != pi
                const auto b = glm::length(glm::cross(newUp, corrUp));  // c * sin(beta)
                const auto b1 =
                    (d.x > 0)
                    ? ((d.y > 0) ? b : -b)
                    : ((d.y > 0) ? -b : b);

                //const glm::quat second (den, 0.f, 0.f, b1);
                return glm::normalize(glm::quat(                        // first * second
                    w * den,
                    d.y * den - d.x * b1,
                    -d.x * den - d.y * b1,
                    w * b1
                ));
            } else {                                                    // cos(beta) == -1, beta == pi
                //const glm::quat second (0.f, 0.f, 0.f, 1.f);
                return glm::normalize(glm::quat( 0.f, -d.x, -d.y, w )); // first * second
            }
        } else {                                                        // up is collinear with d
            //const glm::quat second (1.f, 0.f, 0.f, 0.f);
            return glm::normalize(first);                               // first * second
        }
    } else {                                                            // cos(alpha) == -1, alpha == pi, d = glm::vec3(0, 0, distance)
        // const glm::quat first(0.f, 0.f, 1.f, 0.f);                   // rotate around Y
        // glm::vec3 newUp(0, 1, 0) == origUp
        // glm::vec3 corrUp(up.x, up.y, 0)
        const auto c = std::sqrt(sqr(up.y) + sqr(up.x));

        if(std::isnormal(c)) {                                          // up is not collinear with d
            const auto den = c + up.y;                                  // c * (1 + cos(beta))
            if(den > c * 0.00001f) {                                    // cos(beta) != -1, beta != pi
                //const glm::quat second (den, 0.f, 0.f, up.x);
                return glm::normalize(glm::quat(0.f, up.x, den, 0.f));  // first * second
            } else {                                                    // cos(beta) == -1, beta == pi
                //const glm::quat second (0.f, 0.f, 0.f, 1.f);
                return glm::normalize(glm::quat(0.f, 1.f, 0.f, 0.f));   // first * second
            }
        } else {                                                        // up is collinear with d
            // glm::quat second(1, 0, 0, 0)
            return glm::quat(0.f, 0.f, 1.f, 0.f);                       // first * second
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

    // first rotate around X - nutation
    // then rotate around Y - precession
    
    const auto horiz = std::sqrt(sqr(d.x) + sqr(d.z));
    const auto w1 = distance + horiz; // distance * (1 + cos(nutation))

    if(std::isnormal(horiz)) {
        const auto w2 = horiz - d.z; // horiz * (1 + cos(precession))

        if (w2 > horiz * 0.00001f) {
            return glm::normalize(glm::quat( w1 * w2, d.y * w2, -w1 * d.x, d.y * d.x));
        } else {
            return glm::normalize(glm::quat( 0.f, 0.f, w1, -d.y ));
        }
    } else {
        return glm::normalize(glm::quat( w1, d.y, 0.f, 0.f ));
    }
}

void Object::FPRotate(float precession, float nutation)
{
    rotation = FPRotate(rotation, precession, nutation);
}

glm::quat Object::FPRotate(const glm::quat& init, float precession, float nutation)
{
    const auto sinP = std::sin(precession * 0.5f);
    const auto cosP = std::cos(precession * 0.5f);

    const auto sinN = std::sin(nutation * 0.5f);
    const auto cosN = std::cos(nutation * 0.5f);

    const glm::quat rotP (
        cosP * init.w - sinP * init.y,
		cosP * init.x + sinP * init.z,
		cosP * init.y + sinP * init.w,
		cosP * init.z - sinP * init.x
    );

    const auto rot = glm::normalize(glm::quat(
        rotP.w * cosN - rotP.x * sinN,
		rotP.x * cosN + rotP.w * sinN,
		rotP.y * cosN + rotP.z * sinN,
		rotP.z * cosN - rotP.y * sinN
    ));

    const auto head = GetUp(rot);

    if(head.y >= 0.f) {
        return rot;
    } else { // clamp nutation to [-pi / 2; pi / 2]
        if(std::isnormal(head.y + 1.f)) {
            const auto eye = GetDirection(rot);
            if(eye.y > 0) { // looking up, nutation is > pi / 2
                const auto w = std::sqrt(sqr(head.x) + sqr(head.z)) + head.z;
                if(std::isnormal(w)) {
                    return glm::normalize(glm::quat( w, w, head.x, -head.x ));
                } else {
                    return glm::normalize(glm::quat( 0.f, 0.f, 1.f, -1.f ));
                }
            } else { // looking down, nutation is < -pi / 2
                const auto w = std::sqrt(sqr(head.x) + sqr(head.z)) - head.z;
                if(std::isnormal(w)) {
                    return glm::normalize(glm::quat( w, -w, -head.x, -head.x ));
                } else {
                    return glm::normalize(glm::quat( 0.f, 0.f, 1.f, 1.f ));
                }
            }
        } else { // nutation is pi
            return glm::quat ( -rot.x, rot.w, rot.z, -rot.y );
        }
    }
}

glm::vec3 Object::GetDirection(const glm::quat& r)
{
    return glm::vec3(
       -2.f * (r.w * r.y + r.z * r.x),
        2.f * (r.w * r.x - r.z * r.y),
        sqr(r.x) + sqr(r.y) - sqr(r.w) - sqr(r.z)
    );
}

glm::vec3 Object::GetRight(const glm::quat& r)
{
    return glm::vec3(
        sqr(r.w) + sqr(r.x) - sqr(r.z) - sqr(r.y),
        2.f * (r.y * r.x + r.z * r.w),
        2.f * (r.z * r.x - r.y * r.w)
    );
}

glm::vec3 Object::GetUp(const glm::quat& r)
{
    return glm::vec3(
        2.f * (r.x * r.y - r.w * r.z),
        sqr(r.w) + sqr(r.y) - sqr(r.x) - sqr(r.z),
        2.f * (r.y * r.z + r.w * r.x)
    );
}

} // namespace GUI
