#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "common.h"

class Material;

class HitRecord
{
public:
    glm::dvec3 p;
    glm::dvec3 normal;
    shared_ptr<Material> mat;
    double t;
    bool frontFace;

    void setFaceNormal(const Ray& r, const glm::dvec3& outwardNormal)
    {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outwardNormal` is assumed to have unit length.

        frontFace = glm::dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif //_HITTABLE_H_