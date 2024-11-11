#ifndef _HITTABLE_LIST_H_
#define _HITTABLE_LIST_H_

#include "common.h"
#include "hittable.h"

#include <vector>

class HittableList : public Hittable
{
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object)
    {
        objects.emplace_back(object);
        bbox = Aabb(bbox, object->boundingBox());
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto& object : objects)
        {
            if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
    
    Aabb boundingBox() const override { return bbox; }

private:
    Aabb bbox;
};

#endif//_HITTABLE_LIST_H_