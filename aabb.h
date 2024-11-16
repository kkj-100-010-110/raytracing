#ifndef _AABB_H_
#define _AABB_H_

class Aabb
{
public:
    Interval x, y, z;

    Aabb() {} // The default AABB is empty, since intervals are empty by default.

    Aabb(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) { padToMinimums(); }

    Aabb(const Point& a, const Point& b)
    {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.

        x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

        padToMinimums();
    }

    Aabb(const Aabb& box0, const Aabb& box1)
    {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval& axisInterval(int n) const { return (n == 0) ? x : (n == 1) ? y : z; }

    bool hit(const Ray& r, Interval rayT) const
    {
        const Point& rayOrig = r.origin();
        const glm::dvec3& rayDir = r.direction();

        for (int axis = 0; axis < 3; axis++)
        {
            const Interval& ax = axisInterval(axis);
            const double adinv = 1.0 / rayDir[axis];

            auto t0 = (ax.min - rayOrig[axis]) * adinv;
            auto t1 = (ax.max - rayOrig[axis]) * adinv;

            if (t0 < t1)
            {
                if (t0 > rayT.min) rayT.min = t0;
                if (t1 < rayT.max) rayT.max = t1;
            }
            else
            {
                if (t1 > rayT.min) rayT.min = t1;
                if (t0 < rayT.max) rayT.max = t0;
            }

            if (rayT.max <= rayT.min) return false;
        }
        return true;
    }

    int longestAxis() const
    {
        // Returns the index of the longest axis of the bounding box.

        if (x.size() > y.size()) return x.size() > z.size() ? 0 : 2;
        else return y.size() > z.size() ? 1 : 2;
    }

    static const Aabb empty, universe;

private:
    void padToMinimums()
    {
        // Adjust the AABB so that no side is narrower than some delta, padding if necessary.

        double delta = 0.0001;
        if (x.size() < delta) x = x.expand(delta);
        if (y.size() < delta) y = y.expand(delta);
        if (z.size() < delta) z = z.expand(delta);
    }
};

const Aabb Aabb::empty = Aabb(Interval::empty, Interval::empty, Interval::empty);
const Aabb Aabb::universe = Aabb(Interval::universe, Interval::universe, Interval::universe);

Aabb operator+(const Aabb& bbox, const glm::dvec3& offset) { return Aabb(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z); }

Aabb operator+(const glm::dvec3& offset, const Aabb& bbox) { return bbox + offset; }

#endif//_AABB_H_