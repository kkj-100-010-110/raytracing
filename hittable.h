#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "aabb.h"

class Material;

class HitRecord
{
public:
    Point p;
    glm::dvec3 normal;
    shared_ptr<Material> mat;
    double t;
    double u;
    double v;
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
    virtual Aabb boundingBox() const = 0;
};

class Translate : public Hittable
{
public:
    Translate(shared_ptr<Hittable> object, const glm::dvec3& offset) : object(object), offset(offset) { bbox = object->boundingBox() + offset; }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        // Move the ray backwards by the offset
        Ray offsetR(r.origin() - offset, r.direction(), r.time());

        // Determine whether an intersection exists along the offset ray (and if so, where)
        if (!object->hit(offsetR, rayT, rec))
            return false;

        // Move the intersection point forwards by the offset
        rec.p += offset;

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> object;
    glm::dvec3 offset;
    Aabb bbox;
};

class RotateY : public Hittable
{
public:
    RotateY(shared_ptr<Hittable> object, double angle) : object(object)
    {
        auto radians = glm::radians(angle);
        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
        bbox = object->boundingBox();

        Point min(INF, INF, INF);
        Point max(-INF, -INF, -INF);

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newx = cosTheta * x + sinTheta * z;
                    auto newz = -sinTheta * x + cosTheta * z;

                    glm::dvec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = Aabb(min, max);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {

        // Transform the ray from world space to object space.

        auto origin = Point((cosTheta * r.origin().x) - (sinTheta * r.origin().z)
                            , r.origin().y
                            , (sinTheta * r.origin().x) + (cosTheta * r.origin().z));

        auto direction = glm::dvec3((cosTheta * r.direction().x) - (sinTheta * r.direction().z)
                                    , r.direction().y
                                    , (sinTheta * r.direction().x) + (cosTheta * r.direction().z));

        Ray rotated_r(origin, direction, r.time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!object->hit(rotated_r, rayT, rec))
            return false;

        // Transform the intersection from object space back to world space.

        rec.p = Point((cosTheta * rec.p.x) + (sinTheta * rec.p.z)
                     , rec.p.y
                     , (-sinTheta * rec.p.x) + (cosTheta * rec.p.z));

        rec.normal = glm::dvec3((cosTheta * rec.normal.x) + (sinTheta * rec.normal.z)
                               , rec.normal.y
                               , (-sinTheta * rec.normal.x) + (cosTheta * rec.normal.z));

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> object;
    double sinTheta;
    double cosTheta;
    Aabb bbox;
};

class RotateX : public Hittable
{
public:
    RotateX(shared_ptr<Hittable> object, double angle) : object(object)
    {
        auto radians = glm::radians(angle);
        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
        bbox = object->boundingBox();

        Point min(INF, INF, INF);
        Point max(-INF, -INF, -INF);

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newy = cosTheta * y + sinTheta * z;
                    auto newz = -sinTheta * y + cosTheta * z;

                    glm::dvec3 tester(x, newy, newz);

                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = Aabb(min, max);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {

        // Transform the ray from world space to object space.

        auto origin = Point(r.origin().x
                           , (cosTheta * r.origin().y) - (sinTheta * r.origin().z)
                           , (sinTheta * r.origin().y) + (cosTheta * r.origin().z));

        auto direction = glm::dvec3(r.direction().x
                                   , (cosTheta * r.direction().y) - (sinTheta * r.direction().z)
                                   , (sinTheta * r.direction().y) + (cosTheta * r.direction().z));

        Ray rotated_r(origin, direction, r.time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!object->hit(rotated_r, rayT, rec))
            return false;

        // Transform the intersection from object space back to world space.

        rec.p = Point(rec.p.x
                     , (cosTheta * rec.p.y) + (sinTheta * rec.p.z)
                     , (-sinTheta * rec.p.y) + (cosTheta * rec.p.z));

        rec.normal = glm::dvec3(rec.normal.x
                               , (cosTheta * rec.normal.y) + (sinTheta * rec.normal.z)
                               , (-sinTheta * rec.normal.y) + (cosTheta * rec.normal.z));

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> object;
    double sinTheta;
    double cosTheta;
    Aabb bbox;
};

class RotateZ : public Hittable
{
public:
    RotateZ(shared_ptr<Hittable> object, double angle) : object(object)
    {
        auto radians = glm::radians(angle);
        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
        bbox = object->boundingBox();

        Point min(INF, INF, INF);
        Point max(-INF, -INF, -INF);

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto newx = cosTheta * x + sinTheta * y;
                    auto newy = -sinTheta * x + cosTheta * y;

                    glm::dvec3 tester(newx, newy, z);

                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = Aabb(min, max);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {

        // Transform the ray from world space to object space.

        auto origin = Point((cosTheta * r.origin().x) - (sinTheta * r.origin().y)
                           , (sinTheta * r.origin().x) + (cosTheta * r.origin().y)
                           , r.origin().z);

        auto direction = glm::dvec3((cosTheta * r.direction().x) - (sinTheta * r.direction().y)
                                   , (sinTheta * r.direction().x) + (cosTheta * r.direction().y)
                                   , r.direction().z);

        Ray rotated_r(origin, direction, r.time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!object->hit(rotated_r, rayT, rec))
            return false;

        // Transform the intersection from object space back to world space.

        rec.p = Point((cosTheta * rec.p.x) + (sinTheta * rec.p.y)
                     , (-sinTheta * rec.p.x) + (cosTheta * rec.p.y)
                     , rec.p.z);

        rec.normal = glm::dvec3((cosTheta * rec.normal.x) + (sinTheta * rec.normal.y)
                               , (-sinTheta * rec.normal.x) + (cosTheta * rec.normal.y)
                               , rec.normal.z);

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> object;
    double sinTheta;
    double cosTheta;
    Aabb bbox;
};

#endif//_HITTABLE_H_