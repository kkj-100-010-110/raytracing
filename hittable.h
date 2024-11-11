#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "common.h"
#include "aabb.h"

class Material;

class HitRecord
{
public:
    glm::dvec4 p;
    glm::dvec4 normal;
    shared_ptr<Material> mat;
    double t;
    double u;
    double v;
    bool frontFace;

    void setFaceNormal(const Ray& r, const glm::dvec4& outwardNormal)
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
    virtual bool hit(const Ray& r, Interval rayT, HitRecord &rec) const = 0;
    virtual Aabb boundingBox() const = 0;
};

#endif//_HITTABLE_H_