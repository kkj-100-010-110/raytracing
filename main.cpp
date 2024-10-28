#include "common.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

int main()
{
    /* Timer Begin */
    Timer timer;

    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(color(0.5, 0.5, 0.5, 0.0));
    world.add(make_shared<Sphere>(glm::dvec4(0,-1000,0,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = randomDouble();
            glm::dvec4 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble(), 0.0);

            if ((center - glm::dvec4(4, 0.2, 0, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8)
                {
                    // diffuse
                    auto albedo = randomVector() * randomVector();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    auto center2 = center + glm::dvec4(0, randomDouble(0,.5), 0, 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95)
                {
                    // metal
                    auto albedo = randomVector(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else
                {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(glm::dvec4(0, 1, 0, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1, 0.0));
    world.add(make_shared<Sphere>(glm::dvec4(-4, 1, 0, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5, 0.0), 0.0);
    world.add(make_shared<Sphere>(glm::dvec4(4, 1, 0, 0), 1.0, material3));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = glm::dvec4(13, 2, 3, 0);
    cam.lookAt = glm::dvec4(0, 0, 0, 0);
    cam.vUp = glm::dvec4(0, 1, 0, 0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.render(world);

    /* Timer End */
    timer.elapsed();

    return 0;
}