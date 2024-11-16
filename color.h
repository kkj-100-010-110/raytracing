#ifndef _COLOR_H_
#define _COLOR_H_

#include "interval.h"

using Color = glm::dvec3;

inline double linearToGamma(double linearComponent)
{
    if (linearComponent > 0)
        return std::sqrt(linearComponent);

    return 0;
}

void writeColor(std::ostream& out, const Color& pixelColor)
{
    auto r = pixelColor.x;
    auto g = pixelColor.y;
    auto b = pixelColor.z;

    // Apply a linear to gamma transform for gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel Color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif//_COLOR_H_