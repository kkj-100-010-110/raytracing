#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#include "common.h"

class Interval
{
public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default Interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    double size() const { return max - min; }

    bool contains(double x) const { return min <= x && x <= max; }

    bool surrounds(double x) const { return min < x && x < max; }

    double clamp(double x) const { return std::clamp(x, min, max); }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif//_INTERVAL_H_