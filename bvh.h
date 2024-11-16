#ifndef _BVH_H_
#define _BVH_H_

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm> // std::sort()

class BvhNode : public Hittable
{
public:
    BvhNode(HittableList list) : BvhNode(list.objects, 0, list.objects.size())
    {
        // There's a C++ subtlety here. This constructor (without span indices) creates an
        // implicit copy of the hittable list, which we will modify. The lifetime of the copied
        // list only extends until this constructor exits. That's OK, because we only need to
        // persist the resulting bounding volume hierarchy.
    }

    BvhNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end)
    {
        // Build the bounding box of the span of source objects.
        bbox = Aabb::empty;
        for (size_t objectIndex = start; objectIndex < end; objectIndex++)
            bbox = Aabb(bbox, objects[objectIndex]->boundingBox());

        int axis = bbox.longestAxis();
        auto comparator = (axis == 0) ? boxXcompare : (axis == 1) ? boxYcompare : boxZcompare;
        size_t objectSpan = end - start;
        if (objectSpan == 1)
        {
            left = right = objects[start];
        }
        else if (objectSpan == 2)
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + objectSpan / 2;
            left = make_shared<BvhNode>(objects, start, mid);
            right = make_shared<BvhNode>(objects, mid, end);
        }
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        if (!bbox.hit(r, rayT))
            return false;

        bool hitLeft = left->hit(r, rayT, rec);
        bool hitRight = right->hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

        return hitLeft || hitRight;
    }

    Aabb boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    Aabb bbox;

    static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex)
    {
        auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
        auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
        return aAxisInterval.min < bAxisInterval.min;
    }

    static bool boxXcompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) { return box_compare(a, b, 0); }

    static bool boxYcompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) { return box_compare(a, b, 1); }

    static bool boxZcompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) { return box_compare(a, b, 2); }
};

#endif//_BVH_H_