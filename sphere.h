#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hittable.h"

class Sphere : public Hittable
{
public:
    // Stationary Sphere
    Sphere(const Point& staticCenter, double radius, shared_ptr<Material> mat)
        : center(staticCenter, glm::dvec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat)
    {
        auto rvec = glm::dvec3(radius, radius, radius);
        bbox = Aabb(staticCenter - rvec, staticCenter + rvec);
    }

    // Moving Sphere
    Sphere(const Point& center1, const Point& center2, double radius, shared_ptr<Material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat)
    {
        auto rvec = glm::dvec3(radius, radius, radius);
        Aabb box1(center.at(0) - rvec, center.at(0) + rvec);
        Aabb box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = Aabb(box1, box2);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        Point currentCenter = center.at(r.time());
        glm::dvec3 oc = currentCenter - r.origin();
        auto a = glm::dot(r.direction(), r.direction());
        auto h = glm::dot(r.direction(), oc);
        auto c = glm::dot(oc, oc) - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

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
        glm::dvec3 outwardNormal = (rec.p - currentCenter) / radius;
        rec.setFaceNormal(r, outwardNormal);
        getSphereUv(outwardNormal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;
    Aabb bbox;

    static void getSphereUv(const Point& p, double& u, double& v)
    {
        // p: a given point on the Sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + PI;

        u = phi / (2 * PI);
        v = theta / PI;
    }
};

#endif//_SPHERE_H_