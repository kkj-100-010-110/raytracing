#ifndef _COLOR_H_
#define _COLOR_H_

#include "interval.h"
#include "common.h"

using color = glm::dvec4;

inline double linearToGamma(double linearComponent)
{
    return linearComponent > 0 ? std::sqrt(linearComponent) : 0;
}

void writeColor(std::ostream& out, const color& pixelColor)
{
    auto r = pixelColor.r;
    auto g = pixelColor.g;
    auto b = pixelColor.b;
    auto a = pixelColor.a;

    // Apply a linear to gamma transform for gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);
    a = linearToGamma(a);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    int32_t rByte = static_cast<int32_t>(256 * intensity.clamp(r));
    int32_t gByte = static_cast<int32_t>(256 * intensity.clamp(g));
    int32_t bByte = static_cast<int32_t>(256 * intensity.clamp(b));
    int32_t aByte = static_cast<int32_t>(256 * intensity.clamp(a));

    // Write out the pixel color components.
    out << rByte << " " << gByte << " " << bByte << '\n';
}

#endif//_COLOR_H_