#ifndef _RAY_H_
#define _RAY_H_

#include "common.h"

class Ray
{
public:
    Ray() {}
    Ray(const glm::dvec4& origin, const glm::dvec4& direction)
        : orig(origin), dir(direction) {}

    const glm::dvec4& origin() const { return orig; }
    const glm::dvec4& direction() const { return dir; }
    glm::dvec4 at(double t) const { return orig + t * dir; }

private:
    glm::dvec4 orig;
    glm::dvec4 dir;
};

#endif//_RAY_H_