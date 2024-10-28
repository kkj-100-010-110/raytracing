#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "hittable.h"
#include "material.h"

class Camera
{
public:
    double aspectRatio = 1.0;     // Ratio of image width over height
    int32_t imageWidth = 100;     // Rendered image width in pixel count
    int32_t samplesPerPixel = 10; // Count of random samples for each pixel
    int32_t maxDepth = 10;        // Maximum number of ray bounces into scene

    double vfov = 90; // Vertical view angle(field of view)
    glm::dvec4 lookFrom = glm::dvec4(0.0, 0.0, 0.0, 0.0);   // Camera is looking from
    glm::dvec4 lookAt = glm::dvec4(0.0, 0.0, -1.0, 0.0);    // Camera is looking at
    glm::dvec4 vUp = glm::dvec4(0.0, 1.0, 0.0, 0.0);        // Camera-relative up direction

    double defocusAngle = 0;  // Variation angle of rays through each pixel
    double focusDist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    void render(const Hittable& world)
    {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
        for (int32_t j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (int32_t i = 0; i < imageWidth; i++)
            {
                color pixelColor(0, 0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    Ray r = getRay(i, j);
                    pixelColor += rayColor(r, maxDepth, world);
                }
                writeColor(std::cout, pixelSamplesScale * pixelColor);
            }
        }
        std::clog << "\rDone.                 \n";
    }

  private:
    int32_t imageHeight;        // Rendered image height
    double pixelSamplesScale;   // Color scale factor for a sum of pixel samples
    glm::dvec4 center;          // Camera center
    glm::dvec4 pixelZeroLoc;    // Location of pixel(0, 0)
    glm::dvec4 pixelDeltaU;     // Offset to pixel to the right
    glm::dvec4 pixelDeltaV;     // Offset to pixel below
    glm::dvec4 u, v, w;         // Camera frame basis vectors
    glm::dvec4 defocusDiskU;    // Defocus disk horizontal radius
    glm::dvec4 defocusDiskV;    // Defocus disk vertical radius

    void initialize()
    {
        // Calculate the image height, and ensure that it's at least 1.
        imageHeight = static_cast<int32_t>(imageWidth / aspectRatio);
        imageHeight = std::clamp(imageHeight, 1, imageHeight);

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = lookFrom;

        // Determin viewport dimensions.
        // double focalLength = glm::length(lookFrom - lookAt);
        auto theta = glm::radians(vfov); // degree to radian
        auto h = glm::tan(theta/2);
        // auto viewportHeight = 2 * h * focalLength;
        auto viewportHeight = 2 * h * focusDist;
        double viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = glm::normalize(lookFrom - lookAt);
        glm::dvec3 crossProduct = glm::cross(glm::dvec3(vUp), glm::dvec3(w));
        u = glm::normalize(glm::dvec4(crossProduct, 0.0));
        crossProduct = glm::cross(glm::dvec3(w), glm::dvec3(u));
        v = glm::dvec4(crossProduct, 0.0);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        glm::dvec4 viewportU = viewportWidth * u;   // Vector across viewport horizontal edge
        glm::dvec4 viewportV = viewportHeight * -v; // Vector down viewport vertical edge
        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaU = viewportU / static_cast<double>(imageWidth);
        pixelDeltaV = viewportV / static_cast<double>(imageHeight);
        // Calculate the location of the upper left pixel.
        glm::dvec4 viewportUpperLeft = center - (focusDist * w) - viewportU / 2.0 - viewportV / 2.0;
        pixelZeroLoc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
        // Calculate the camera defocus disk basis vectors.
        auto defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    Ray getRay(int32_t i, int32_t j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.
        auto offset = sampleSquare();
        auto pixelSample = pixelZeroLoc + ((i + offset.x) * pixelDeltaU) + ((j + offset.y) * pixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();;
        auto rayDirection = pixelSample - rayOrigin;
        auto rayTime = randomDouble();

        return Ray(rayOrigin, rayDirection, rayTime);
    }

    glm::dvec4 sampleSquare() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return glm::dvec4(randomDouble() - 0.5, randomDouble() - 0.5, 0, 0);
    }

    glm::dvec4 defocusDiskSample() const {
        // Returns a random point in the camera defocus disk.
        auto p = randomInUnitDisk();
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    color rayColor(const Ray& r, int32_t depth, const Hittable& world) const
    {
        if (depth <= 0)
            return color(0, 0, 0, 0);

        HitRecord rec;
        if (world.hit(r, Interval(0.001, infinity), rec))
        {
            Ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * rayColor(scattered, depth-1, world);
            return color(0,0,0,0);
        }

        glm::dvec4 unitDirection = glm::normalize(r.direction());
        auto a = 0.5 * (unitDirection.y + 1.0);

        return lerp(color(1.0, 1.0, 1.0, 0.0), color(0.5, 0.7, 1.0, 0.0), a);
    }
};

#endif//_CAMERA_H_