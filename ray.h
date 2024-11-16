#ifndef _RAY_H_
#define _RAY_H_

class Ray
{
public:
    Ray() {}
    Ray(const Point& origin, const glm::dvec3& direction, double time) : orig(origin), dir(direction), tm(time) {}
    Ray(const Point& origin, const glm::dvec3& direction) : Ray(origin, direction, 0) {}
    const Point& origin() const { return orig; }
    const glm::dvec3& direction() const { return dir; }
    double time() const { return tm; }
    Point at(double t) const { return orig + t * dir; }

private:
    Point orig;
    glm::dvec3 dir;
    double tm;
};

#endif//_RAY_H_