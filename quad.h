#ifndef _QUAD_H_
#define _QUAD_H_

#include "hittable.h"
#include "hittable_list.h"

class Quad : public Hittable
{
public:
    Quad(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat) : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = glm::cross(u, v);
        normal = glm::normalize(n);
        D = glm::dot(normal, Q);
        w = n / glm::dot(n, n);

        setBoundingBox();
    }

    virtual void setBoundingBox()
    {
        // Compute the bounding box of all four vertices.
        auto bboxDiagonal1 = Aabb(Q, Q + u + v);
        auto bboxDiagonal2 = Aabb(Q + u, Q + v);
        bbox = Aabb(bboxDiagonal1, bboxDiagonal2);
    }

    Aabb boundingBox() const override { return bbox; }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denominator) < 1e-8) return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - glm::dot(normal, r.origin())) / denominator;
        if (!rayT.contains(t)) return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        glm::dvec3 planarHitPtVector = intersection - Q;
        auto alpha = glm::dot(w, cross(planarHitPtVector, v));
        auto beta = glm::dot(w, cross(u, planarHitPtVector));

        if (!isInterior(alpha, beta, rec)) return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord& rec) const
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unitInterval.contains(a) || !unitInterval.contains(b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    Point Q;
    glm::dvec3 u, v;
    glm::dvec3 w;
    shared_ptr<Material> mat;
    Aabb bbox;
    glm::dvec3 normal;
    double D;
};

inline shared_ptr<HittableList> box(const Point& a, const Point& b, shared_ptr<Material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<HittableList>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
    auto max = Point(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));

    auto dx = glm::dvec3(max.x - min.x, 0, 0);
    auto dy = glm::dvec3(0, max.y - min.y, 0);
    auto dz = glm::dvec3(0, 0, max.z - min.z);

    sides->add(make_shared<Quad>(Point(min.x, min.y, max.z), dx, dy, mat));  // front
    sides->add(make_shared<Quad>(Point(max.x, min.y, max.z), -dz, dy, mat)); // right
    sides->add(make_shared<Quad>(Point(max.x, min.y, min.z), -dx, dy, mat)); // back
    sides->add(make_shared<Quad>(Point(min.x, min.y, min.z), dz, dy, mat));  // left
    sides->add(make_shared<Quad>(Point(min.x, max.y, max.z), dx, -dz, mat)); // top
    sides->add(make_shared<Quad>(Point(min.x, min.y, min.z), dx, dz, mat));  // bottom

    return sides;
}

#endif