#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hittable.h"

class Sphere : public Hittable
{
public:
    // Stationary Sphere
    Sphere(const glm::dvec4 &staticCenter, double radius, shared_ptr<Material> mat)
        : center(staticCenter, glm::dvec4(0, 0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}

    // Moving Sphere
    Sphere(const glm::dvec4 &center1, const glm::dvec4 &center2, double radius, shared_ptr<Material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}


    bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override
    {
        glm::dvec4 currentCenter = center.at(r.time());
        glm::dvec4 oc = currentCenter - r.origin();
        auto a = glm::dot(r.direction(), r.direction());
        auto h = glm::dot(r.direction(), oc);
        auto c = glm::dot(oc, oc) - radius * radius;
        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;
        auto sqrtd = glm::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!rayT.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!rayT.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        glm::dvec4 outwardNormal = (rec.p - currentCenter) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }

private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;
};

#endif//_SPHERE_H_