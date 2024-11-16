#include "common.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    // /* Timer Begin */
    // Timer timer;

    // HittableList world;

    // auto materialGround = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    // auto materialCenter = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    // auto materialLeft   = make_shared<Dielectric>(1.50);
    // auto materialBubble = make_shared<Dielectric>(1.00 / 1.50);
    // auto materialRight  = make_shared<Metal>(color(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<Sphere>(glm::dvec3( 0.0, -100.5, -1.0), 100.0, materialGround));
    // world.add(make_shared<Sphere>(glm::dvec3( 0.0,    0.0, -1.2),   0.5, materialCenter));
    // world.add(make_shared<Sphere>(glm::dvec3(-1.0,    0.0, -1.0),   0.5, materialLeft));
    // world.add(make_shared<Sphere>(glm::dvec3(-1.0,    0.0, -1.0),   0.4, materialBubble));
    // world.add(make_shared<Sphere>(glm::dvec3( 1.0,    0.0, -1.0),   0.5, materialRight));

    // Camera cam;

    // cam.aspectRatio      = 16.0 / 9.0;
    // cam.imageWidth       = 400;
    // cam.samplesPerPixel = 100;
    // cam.maxDepth         = 50;

    // cam.vfov     = 20;
    // cam.lookFrom = glm::dvec3(-2, 2, 1, 0.0);
    // cam.lookAt   = glm::dvec3(0, 0, -1, 0.0);
    // cam.vUp      = glm::dvec3(0, 1, 0, 0);

    // cam.defocusAngle = 10.0;
    // cam.focusDist    = 3.4;

    // cam.render(world);

    // /* Timer End */
    // timer.elapsed();

    /* Timer Begin */
    Timer timer;

    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(glm::dvec3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = randomDouble();
            glm::dvec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - glm::dvec3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8)
                {
                    // diffuse
                    auto albedo = randomVector() * randomVector();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
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
    world.add(make_shared<Sphere>(glm::dvec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(glm::dvec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(glm::dvec3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 1200;
    cam.samplesPerPixel = 500;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = glm::dvec3(13, 2, 3);
    cam.lookAt = glm::dvec3(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.render(world);

    /* Timer End */
    timer.elapsed();

    return 0;
}