#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "hittable.h"

class Triangle : public Hittable
{
public:
    Triangle(const glm::dvec4& Q, const glm::dvec4& u, const glm::dvec4& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        glm::dvec3 n = glm::cross(glm::dvec3(u), glm::dvec3(v));
        normal = glm::normalize(glm::dvec4(n, 0.0));
        d = glm::dot(normal, Q);
        w = glm::dvec4(n, 0.0) / glm::dot(n, n);

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

    bool hit(const Ray& r, Interval rayT, HitRecord &rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denominator) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (d - glm::dot(normal, r.origin())) / denominator;
        if (!rayT.contains(t))
            return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        glm::dvec4 planarHitPtVector = intersection - Q;
        glm::dvec3 alphaNumerator = glm::cross(glm::dvec3(planarHitPtVector), glm::dvec3(v));
        glm::dvec3 betaNumerator = glm::cross(glm::dvec3(u), glm::dvec3(planarHitPtVector));
        auto alpha = glm::dot(w, glm::dvec4(alphaNumerator, 0));
        auto beta = glm::dot(w, glm::dvec4(betaNumerator, 0));

        if (!isInterior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord &rec) const
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!(a > 0 && b > 0 && a + b < 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    glm::dvec4 Q;
    glm::dvec4 u, v;
    shared_ptr<Material> mat;
    Aabb bbox;
    glm::dvec4 normal;
    double d;
    glm::dvec4 w;
};

class Circle : public Hittable
{
public:
    Circle(const glm::dvec4& Q, const glm::dvec4& u, const glm::dvec4& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        glm::dvec3 n = glm::cross(glm::dvec3(u), glm::dvec3(v));
        normal = glm::normalize(glm::dvec4(n, 0.0));
        d = glm::dot(normal, Q);
        w = glm::dvec4(n, 0.0) / glm::dot(n, n);

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

    bool hit(const Ray& r, Interval rayT, HitRecord &rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denominator) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (d - glm::dot(normal, r.origin())) / denominator;
        if (!rayT.contains(t))
            return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        glm::dvec4 planarHitPtVector = intersection - Q;
        glm::dvec3 alphaNumerator = glm::cross(glm::dvec3(planarHitPtVector), glm::dvec3(v));
        glm::dvec3 betaNumerator = glm::cross(glm::dvec3(u), glm::dvec3(planarHitPtVector));
        auto alpha = glm::dot(w, glm::dvec4(alphaNumerator, 0));
        auto beta = glm::dot(w, glm::dvec4(betaNumerator, 0));

        if (!isInterior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord &rec) const
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        double radius = 1;

        if (!(a * a + b * b < radius * radius))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    glm::dvec4 Q;
    glm::dvec4 u, v;
    shared_ptr<Material> mat;
    Aabb bbox;
    glm::dvec4 normal;
    double d;
    glm::dvec4 w;
};

class Annuli : public Hittable
{
public:
    Annuli(const glm::dvec4& Q, const glm::dvec4& u, const glm::dvec4& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        glm::dvec3 n = glm::cross(glm::dvec3(u), glm::dvec3(v));
        normal = glm::normalize(glm::dvec4(n, 0.0));
        d = glm::dot(normal, Q);
        w = glm::dvec4(n, 0.0) / glm::dot(n, n);

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

    bool hit(const Ray& r, Interval rayT, HitRecord &rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denominator) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (d - glm::dot(normal, r.origin())) / denominator;
        if (!rayT.contains(t))
            return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        glm::dvec4 planarHitPtVector = intersection - Q;
        glm::dvec3 alphaNumerator = glm::cross(glm::dvec3(planarHitPtVector), glm::dvec3(v));
        glm::dvec3 betaNumerator = glm::cross(glm::dvec3(u), glm::dvec3(planarHitPtVector));
        auto alpha = glm::dot(w, glm::dvec4(alphaNumerator, 0));
        auto beta = glm::dot(w, glm::dvec4(betaNumerator, 0));

        if (!isInterior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord &rec) const
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        double distanceSquared = a * a + b * b;
        double innerRadius = 0.5;
        double outerRadius = 1;
        if (!(distanceSquared >= innerRadius * innerRadius && distanceSquared <= outerRadius * outerRadius))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    glm::dvec4 Q;
    glm::dvec4 u, v;
    shared_ptr<Material> mat;
    Aabb bbox;
    glm::dvec4 normal;
    double d;
    glm::dvec4 w;
};

class Ellipses : public Hittable
{
public:
    Ellipses(const glm::dvec4& Q, const glm::dvec4& u, const glm::dvec4& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        glm::dvec3 n = glm::cross(glm::dvec3(u), glm::dvec3(v));
        normal = glm::normalize(glm::dvec4(n, 0.0));
        d = glm::dot(normal, Q);
        w = glm::dvec4(n, 0.0) / glm::dot(n, n);

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

    bool hit(const Ray& r, Interval rayT, HitRecord &rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denominator) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (d - glm::dot(normal, r.origin())) / denominator;
        if (!rayT.contains(t))
            return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        glm::dvec4 planarHitPtVector = intersection - Q;
        glm::dvec3 alphaNumerator = glm::cross(glm::dvec3(planarHitPtVector), glm::dvec3(v));
        glm::dvec3 betaNumerator = glm::cross(glm::dvec3(u), glm::dvec3(planarHitPtVector));
        auto alpha = glm::dot(w, glm::dvec4(alphaNumerator, 0));
        auto beta = glm::dot(w, glm::dvec4(betaNumerator, 0));

        if (!isInterior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord &rec) const
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        double radiusX = 0.5;
        double radiusY = 1.5;
        if (!((a * a) / (radiusX * radiusX) + (b * b) / (radiusY * radiusY) < 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    glm::dvec4 Q;
    glm::dvec4 u, v;
    shared_ptr<Material> mat;
    Aabb bbox;
    glm::dvec4 normal;
    double d;
    glm::dvec4 w;
};

#endif//_PRIMITIVES_H_