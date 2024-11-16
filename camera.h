#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "hittable.h"
#include "material.h"

class Camera
{
public:
    double aspectRatio = 1.0;  // Ratio of image width over height
    int imageWidth = 100;      // Rendered image width in pixel count
    int samplesPerPixel = 10;  // Count of random samples for each pixel
    int maxDepth = 10;         // Maximum number of ray bounces into scene
    bool useBackground = true; // Check whether to use background or sky color linear interpolation
    Color background;          // Scene background color

    double verticalFov = 90;              // Vertical view angle (field of view)
    Point lookFrom = Point(0, 0, 0);      // Point camera is looking from
    Point lookAt = Point(0, 0, -1);       // Point camera is looking at
    glm::dvec3 vUp = glm::dvec3(0, 1, 0); // Camera-relative "up" direction

    double defocusAngle = 0; // Variation angle of rays through each pixel
    double focusDist = 10;   // Distance from camera lookFrom point to plane of perfect focus

    void render(const Hittable& world)
    {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; i++)
            {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    Ray r = getRay(i, j);
                    pixel_color += rayColor(r, maxDepth, world);
                }
                writeColor(std::cout, pixelSamplesScale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int imageHeight;          // Rendered image height
    double pixelSamplesScale; // Color scale factor for a sum of pixel samples
    Point center;             // Camera center
    Point pixeZeroLoc;        // Location of pixel 0, 0
    glm::dvec3 pixelDeltaU;   // Offset to pixel to the right
    glm::dvec3 pixelDeltaV;   // Offset to pixel below
    glm::dvec3 u, v, w;       // Camera frame basis vectors
    glm::dvec3 defocusDiskU;  // Defocus disk horizontal radius
    glm::dvec3 defocusDiskV;  // Defocus disk vertical radius

    void initialize()
    {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = lookFrom;

        // Determine viewport dimensions.
        auto theta = glm::radians(verticalFov);
        auto h = std::tan(theta / 2);
        auto viewportHeight = 2 * h * focusDist;
        auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = glm::normalize(lookFrom - lookAt);
        u = glm::normalize(glm::cross(vUp, w));
        v = glm::cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        glm::dvec3 viewportU = viewportWidth * u;   // Vector across viewport horizontal edge
        glm::dvec3 viewportV = viewportHeight * -v; // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaU = viewportU / static_cast<double>(imageWidth);
        pixelDeltaV = viewportV / static_cast<double>(imageHeight);

        // Calculate the location of the upper left pixel.
        auto viewportUpperLeft = center - (focusDist * w) - viewportU / 2.0 - viewportV / 2.0;
        pixeZeroLoc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate the camera defocus disk basis vectors.
        auto defocusRadius = focusDist * std::tan(glm::radians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    Ray getRay(int i, int j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = sampleSquare();
        auto pixelSample = pixeZeroLoc + ((i + offset.x) * pixelDeltaU) + ((j + offset.y) * pixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0) ? center : defocus_disk_sample();
        auto rayDirection = pixelSample - rayOrigin;
        auto CheckerTextureIme = randomDoubleGen();

        return Ray(rayOrigin, rayDirection, CheckerTextureIme);
    }

    glm::dvec3 sampleSquare() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return glm::dvec3(randomDoubleGen() - 0.5, randomDoubleGen() - 0.5, 0);
    }

    glm::dvec3 sampleDisk(double radius) const
    {
        // Returns a random point in the unit (radius 0.5) disk centered at the origin.
        return radius * randomInUnitDisk();
    }

    Point defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        auto p = randomInUnitDisk();
        return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    Color rayColor(const Ray& r, int depth, const Hittable& world) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return Color(0, 0, 0);

        HitRecord rec;

        if (useBackground)
        {
            // If the ray hits nothing, return the background color.
            if (!world.hit(r, Interval(0.001, INF), rec))
                return background;

            Ray scattered;
            Color attenuation;
            Color colorFromEmission = rec.mat->emitted(rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, attenuation, scattered))
                return colorFromEmission;

            Color colorFromScatter = attenuation * rayColor(scattered, depth - 1, world);

            return colorFromEmission + colorFromScatter;
        }
        else
        {
            if (world.hit(r, Interval(0.001, INF), rec))
            {
                Ray scattered;
                Color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * rayColor(scattered, depth - 1, world);
                return Color(0, 0, 0);
            }

            glm::dvec3 unitDirection = glm::normalize(r.direction());
            auto a = 0.5 * (unitDirection.y + 1.0);

            return lerp(Color(1.0, 1.0, 1.0), Color(0.5, 0.7, 1.0), a);
        }
    }
};

#endif//_CAMERA_H_