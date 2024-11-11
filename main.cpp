#include "common.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "quad.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "texture.h"

/* Scenes */
void boundingSpheres();
void checkeredSpheres();
void earth();
void perlinSpheres();
void quads();

int main()
{
    /* Timer Begin */
    Timer timer;

    switch (5)
    {
        case 1: boundingSpheres(); break;
        case 2: checkeredSpheres(); break;
        case 3: earth(); break;
        case 4: perlinSpheres(); break;
        case 5: quads(); break;
    }

    /* Timer End */
    timer.elapsed();

    return 0;
}

void boundingSpheres()
{
    HittableList world;

    // auto groundMaterial = make_shared<Lambertian>(color(0.5, 0.5, 0.5, 0.0));
    // world.add(make_shared<Sphere>(glm::dvec4(0,-1000,0,0), 1000, groundMaterial));
    auto checker = make_shared<CheckerTexture>(0.32, color(.2, .3, .1, 0), color(.9, .9, .9, 0));
    world.add(make_shared<Sphere>(glm::dvec4(0,-1000,0,0), 1000, make_shared<Lambertian>(checker)));


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

    world = HittableList(make_shared<BvhNode>(world));

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
}

void checkeredSpheres()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32, color(.2, .3, .1, .0), color(.9, .9, .9, .0));

    world.add(make_shared<Sphere>(glm::dvec4(0, -10, 0, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(glm::dvec4(0, 10, 0, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = glm::dvec4(13, 2, 3, 0);
    cam.lookAt = glm::dvec4(0, 0, 0, 0);
    cam.vUp = glm::dvec4(0, 1, 0, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void earth()
{
    auto earthTexture = make_shared<ImageTexture>("./image/earthmap.jpg");
    auto earthSurface = make_shared<Lambertian>(earthTexture);
    auto globe = make_shared<Sphere>(glm::dvec4(0, 0, 0, 0), 2, earthSurface);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = glm::dvec4(0, 0, 12, 0);
    cam.lookAt = glm::dvec4(0, 0, 0, 0);
    cam.vUp = glm::dvec4(0, 1, 0, 0);

    cam.defocusAngle = 0;

    cam.render(HittableList(globe));
}

void perlinSpheres()
{
    HittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(glm::dvec4(0, -1000, 0, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(glm::dvec4(0, 2, 0, 0), 2, make_shared<Lambertian>(pertext)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = glm::dvec4(13, 2, 3, 0);
    cam.lookAt = glm::dvec4(0, 0, 0, 0);
    cam.vUp = glm::dvec4(0, 1, 0, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void quads()
{
    HittableList world;

    // Materials
    auto leftRed = make_shared<Lambertian>(color(1.0, 0.2, 0.2, 0.0));
    auto backGreen = make_shared<Lambertian>(color(0.2, 1.0, 0.2, 0.0));
    auto rightBlue = make_shared<Lambertian>(color(0.2, 0.2, 1.0, 0.0));
    auto upperOrange = make_shared<Lambertian>(color(1.0, 0.5, 0.0, 0.0));
    auto lowerTeal = make_shared<Lambertian>(color(0.2, 0.8, 0.8, 0.0));

    // Quads
    world.add(make_shared<Quad>(glm::dvec4(-3, -2, 5, 0), glm::dvec4(0, 0, -4, 0), glm::dvec4(0, 4, 0, 0), leftRed));
    world.add(make_shared<Quad>(glm::dvec4(-2, -2, 0, 0), glm::dvec4(4, 0, 0, 0), glm::dvec4(0, 4, 0, 0), backGreen));
    world.add(make_shared<Quad>(glm::dvec4(3, -2, 1, 0), glm::dvec4(0, 0, 4, 0), glm::dvec4(0, 4, 0, 0), rightBlue));
    world.add(make_shared<Quad>(glm::dvec4(-2, 3, 1, 0), glm::dvec4(4, 0, 0, 0), glm::dvec4(0, 0, 4, 0), upperOrange));
    world.add(make_shared<Quad>(glm::dvec4(-2, -3, 5, 0), glm::dvec4(4, 0, 0, 0), glm::dvec4(0, 0, -4, 0), lowerTeal));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vfov = 80;
    cam.lookFrom = glm::dvec4(0, 0, 9, 0);
    cam.lookAt = glm::dvec4(0, 0, 0, 0);
    cam.vUp = glm::dvec4(0, 1, 0, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}