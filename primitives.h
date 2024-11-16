#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "quad.h"

class Triangle : public Quad
{
public:
    Triangle(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat) : Quad(Q, u, v, mat) {}

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
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
};

class Circle : public Quad
{
public:
    Circle(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat, double radius)
        : Quad(Q, u, v, mat), radius(radius)
    {}

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!(a * a + b * b < radius * radius))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    double radius;
};

class Annuli : public Quad
{
public:
    Annuli(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat, double innerRadius, double outerRadius)
        : Quad(Q, u, v, mat), innerRadius(innerRadius), outerRadius(outerRadius)
    {}

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        double distanceSquared = a * a + b * b;
        if (!(distanceSquared >= innerRadius * innerRadius && distanceSquared <= outerRadius * outerRadius))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    double innerRadius;
    double outerRadius;
};

class Ellipse : public Quad
{
public:
    Ellipse(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat, double radiusX, double radiusY)
        : Quad(Q, u, v, mat), radiusX(radiusX), radiusY(radiusY)
    {}

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!((a * a) / (radiusX * radiusX) + (b * b) / (radiusY * radiusY) < 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    double radiusX;
    double radiusY;
};

#endif//_PRIMITIVES_H_