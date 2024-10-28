#ifndef _RAY_H_
#define _RAY_H_

#include "common.h"

class Ray
{
public:
    Ray() {}
    Ray(const glm::dvec4& origin, const glm::dvec4& direction, double time)
        : orig(origin), dir(direction), tm(time) {}

    Ray(const glm::dvec4& origin, const glm::dvec4& direction)
        : Ray(origin, direction, 0) {}

    double time() const { return tm; }
    const glm::dvec4& origin() const { return orig; }
    const glm::dvec4& direction() const { return dir; }
    glm::dvec4 at(double t) const { return orig + t * dir; }

private:
    glm::dvec4 orig;
    glm::dvec4 dir;
    double tm;
};

#endif//_RAY_H_