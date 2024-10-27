#ifndef _COMMON_H_
#define _COMMON_H_

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <algorithm>
#include <chrono> // for Timer class

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

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

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = glm::pi<double>();

// Utility Functions
/* Linear interpolation */
inline glm::dvec4 lerp(const glm::dvec4& v1, const glm::dvec4& v2, double t)
{
    return (1 - t) * v1 + t * v2;
}
/* Here I use GLM, so glm::radian(degree) will replace this function */
inline double degreesToRadians(double degrees)
{
    return degrees * pi / 180.0;
}
/* Keep updating this function */
inline double randomDouble(double min = 0.0, double max = 1.0)
{
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}
/* Return a 4D(but it uses only 3 components) vector with random values */
inline glm::dvec4 randomVector(double min = 0.0, double max = 1.0)
{
    return glm::dvec4(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max), 0);
}
/* Return a 4D(but it uses only 3 components) unit vector for a random vector */
inline glm::dvec4 randomUnitVector()
{
    while (true)
    {
        glm::dvec4 p = randomVector(-1, 1);
        auto lengthSquare = glm::dot(p, p);
        if (1e-160 < lengthSquare && lengthSquare <= 1)
            return glm::normalize(p); // p / sqrt(lengthSquare);
    }
}
/* Return a 4D(but it uses only 3 components) vector that is on the hemisphere */
inline glm::dvec4 randomOnHemisphere(const glm::dvec4& normal)
{
    glm::dvec4 onUnitSphere = randomUnitVector();
    if (glm::dot(onUnitSphere, normal) > 0.0) return onUnitSphere;
    else return -onUnitSphere;
}

inline bool nearZero(const glm::dvec4& vec)
{
    auto epsilon = 1e-8;
    // return glm::all(glm::epsilonEqual(vec, glm::dvec4(0.0), epsilon)); // slower than below
    return glm::dot(vec, vec) < epsilon * epsilon;
}

inline glm::dvec4 refract(const glm::dvec4& r, const glm::dvec4& n, double etaiOverEtat)
{
    auto cosTheta = std::fmin(dot(-r, n), 1.0);
    glm::dvec4 rOutPerpendicular =  etaiOverEtat * (r + cosTheta * n);
    glm::dvec4 rOutParallel = -std::sqrt(std::fabs(1.0 - glm::dot(rOutPerpendicular, rOutPerpendicular))) * n;
    return rOutPerpendicular + rOutParallel;
}

inline glm::dvec4 randomInUnitDisk()
{
    while (true)
    {
        auto p = glm::dvec4(randomDouble(-1, 1), randomDouble(-1, 1), 0, 0);
        if (glm::dot(p, p) < 1)
            return p;
    }
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "interval.h"

#endif//_COMMON_H_