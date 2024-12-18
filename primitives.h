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

class Parabola : public Quad
{
public:
    Parabola(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat, double p, bool vertical)
        : Quad(Q, u, v, mat), p(p), vertical(vertical)
    {}

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (vertical)
        {
            if (!((a * a) < 4 * p * b))
                return false;
        }
        else
        {
            if (!((b * b) < 4 * p * a))
                return false;
        }

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    double p;
    bool vertical;
};

class Hyperbola : public Quad
{
public:
    Hyperbola(const Point& Q, const glm::dvec3& u, const glm::dvec3& v, shared_ptr<Material> mat, double c, double len)
        : Quad(Q, u, v, mat), c(c), len(len)
    {
        if (c < len || len < 0)
        {
            std::cerr << "c should be longer than len and len should be bigger than 0." << std::endl;
            condition = false;
        }
        else
        {
            condition = true;
            d = (c * c) - (len * len);
        }
    }

    virtual bool isInterior(double a, double b, HitRecord& rec) const override
    {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (condition)
        {
            if (!((a * a) / (len * len) - (b * b) / d > 1))
                return false;
        }

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    bool condition;
    double c;
    double d;
    double len;
};

#endif//_PRIMITIVES_H_