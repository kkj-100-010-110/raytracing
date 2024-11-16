#ifndef _COMMON_H_
#define _COMMON_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory> // std::shared_ptr(), std::make_shared()
#include <chrono> // Timer

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

/* C++ Std Usings */
using std::make_shared;
using std::shared_ptr;

/* Constants */
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

/* Timer */
class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using secont_t = std::chrono::duration<double, std::ratio<1>>;

    // as soon as an object is created, it records the start time.
    std::chrono::time_point<clock_t> start_time = clock_t::now();

public:
    void elapsed()
    {
        std::chrono::time_point<clock_t> end_time = clock_t::now();

        std::clog << "It takes ";
        // as soon as elapsed() is called, it records end_time and print end_time - start_time.
        std::clog << std::chrono::duration_cast<secont_t>(end_time - start_time).count() << '\n' << std::flush;
    }
};

/* Maths & Utilities */

// Point is just an alias for glm::dvec4, but useful for geometric clarity in the code.
using Point = glm::dvec3;

inline double randomDoubleGen()
{
    // Returns a random real in [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDoubleGen(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * randomDoubleGen();
}

inline int randomIntGen(int min, int max)
{
    // Returns a random integer in [min,max].
    return int(randomDoubleGen(min, max + 1));
}

inline bool nearZero(glm::dvec3& vec)
{
    auto epsilon = 1e-8;
    return (std::fabs(vec.x) < epsilon) && (std::fabs(vec.y) < epsilon) && (std::fabs(vec.z) < epsilon);
}

inline glm::dvec3 randomVectorGen()
{
    return glm::dvec3(randomDoubleGen(), randomDoubleGen(), randomDoubleGen());
}

inline glm::dvec3 randomVectorGen(double min, double max)
{
    return glm::dvec3(randomDoubleGen(min, max), randomDoubleGen(min, max), randomDoubleGen(min, max));
}

inline glm::dvec3 randomInUnitDisk()
{
    while (true)
    {
        auto p = glm::dvec3(randomDoubleGen(-1, 1), randomDoubleGen(-1, 1), 0);
        if (glm::dot(p, p) < 1)
            return p;
    }
}

glm::dvec3 lerp(const glm::dvec3& v1, const glm::dvec3& v2, double t) { return (1 - t) * v1 + t * v2; }

inline glm::dvec3 randomUnitVectorGen()
{
    while (true)
    {
        auto p = randomVectorGen(-1, 1);
        auto pLength = glm::dot(p, p);
        if (1e-160 < pLength && pLength <= 1.0)
            return p / std::sqrt(pLength);
    }
}

inline glm::dvec3 randomOnHemisphere(const glm::dvec3& normal)
{
    glm::dvec3 onUnitSphere = randomUnitVectorGen();
    if (glm::dot(onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return onUnitSphere;
    else
        return -onUnitSphere;
}

inline glm::dvec3 reflect(const glm::dvec3& v, const glm::dvec3& n) { return v - 2 * glm::dot(v, n) * n; }

inline glm::dvec3 refract(const glm::dvec3& uv, const glm::dvec3& n, double etaiOverEtat)
{
    auto cosTheta = std::fmin(glm::dot(-uv, n), 1.0);
    glm::dvec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    glm::dvec3 rOutParallel = -std::sqrt(std::fabs(1.0 - glm::dot(rOutPerp, rOutPerp))) * n;
    return rOutPerp + rOutParallel;
}

// replace this function with glm::radians()
inline double degreesToRadians(double degrees) { return degrees * PI / 180.0; }

/* Common Headers */
#include "color.h"
#include "interval.h"
#include "ray.h"

#endif//_COMMON_H_