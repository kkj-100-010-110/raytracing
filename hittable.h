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

class translate : public Hittable
{
public:
    translate(shared_ptr<Hittable> object, const glm::dvec3& offset) : object(object), offset(offset) { bbox = object->boundingBox() + offset; }

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

class rotate_y : public Hittable
{
public:
    rotate_y(shared_ptr<Hittable> object, double angle) : object(object)
    {
        auto radians = glm::radians(angle);
        sin_theta = std::sin(radians);
        cos_theta = std::cos(radians);
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

                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

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

        auto origin = Point((cos_theta * r.origin().x) - (sin_theta * r.origin().z), r.origin().y,
                             (sin_theta * r.origin().x) + (cos_theta * r.origin().z));

        auto direction = glm::dvec3((cos_theta * r.direction().x) - (sin_theta * r.direction().z), r.direction().y,
                              (sin_theta * r.direction().x) + (cos_theta * r.direction().z));

        Ray rotated_r(origin, direction, r.time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!object->hit(rotated_r, rayT, rec))
            return false;

        // Transform the intersection from object space back to world space.

        rec.p = Point((cos_theta * rec.p.x) + (sin_theta * rec.p.z), rec.p.y, (-sin_theta * rec.p.x) + (cos_theta * rec.p.z));

        rec.normal = glm::dvec3((cos_theta * rec.normal.x) + (sin_theta * rec.normal.z), rec.normal.y,
                          (-sin_theta * rec.normal.x) + (cos_theta * rec.normal.z));

        return true;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    Aabb bbox;
};

#endif//_HITTABLE_H_