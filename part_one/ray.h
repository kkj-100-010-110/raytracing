#ifndef _RAY_H_
#define _RAY_H_

#include "common.h"

class Ray
{
public:
    Ray() {}
    Ray(const glm::dvec3& origin, const glm::dvec3& direction) : orig(origin), dir(direction) {}

    const glm::dvec3& origin() const { return orig; }
    const glm::dvec3& direction() const { return dir; }
    glm::dvec3 at(double t) const { return orig + t * dir; }

private:
    glm::dvec3 orig;
    glm::dvec3 dir;
};

#endif //_RAY_H_