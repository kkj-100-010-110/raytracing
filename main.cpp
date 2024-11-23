#include "common.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "primitives.h"
#include "sphere.h"
#include "texture.h"

void bouncingSpheres();
void checkeredSpheres();
void earth();
void perlinSpheres();
void quads();
void primitives();
void simpleLight();
void cornellBox();
void cornellSmoke();
void finalScene(int imageWidth, int samplesPerPixel, int maxDepth);
void test();

int main()
{
    Timer timer;

    switch (10)
    {
        case 1: bouncingSpheres(); break;
        case 2: checkeredSpheres(); break;
        case 3: earth(); break;
        case 4: perlinSpheres(); break;
        case 5: quads(); break;
        case 6: primitives(); break;
        case 7: simpleLight(); break;
        case 8: cornellBox(); break;
        case 9: cornellSmoke(); break;
        case 10: finalScene(800, 10000, 40); break;
        case 11: test(); break;
        default: test(); break;
    }

    timer.elapsed();
    return 0;
}

void bouncingSpheres()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = randomDoubleGen();
            Point center(a + 0.9 * randomDoubleGen(), 0.2, b + 0.9 * randomDoubleGen());

            if ((center - Point(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8)
                {
                    // diffuse
                    auto albedo = randomVectorGen() * randomVectorGen();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    auto center2 = center + glm::dvec3(0, randomDoubleGen(0, .5), 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95)
                {
                    // metal
                    auto albedo = randomVectorGen(0.5, 1);
                    auto fuzz = randomDoubleGen(0, 0.5);
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
    world.add(make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    world = HittableList(make_shared<BvhNode>(world));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 20;
    cam.lookFrom = Point(13, 2, 3);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.render(world);
}

void checkeredSpheres()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

    world.add(make_shared<Sphere>(Point(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 20;
    cam.lookFrom = Point(13, 2, 3);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void earth()
{
    auto earthTexture = make_shared<ImageTexture>("./image/earthmap.jpg");
    auto earthSurface = make_shared<Lambertian>(earthTexture);
    auto globe = make_shared<Sphere>(Point(0, 0, 0), 2, earthSurface);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 20;
    cam.lookFrom = Point(0, 0, 12);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(HittableList(globe));
}

void perlinSpheres()
{
    HittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 20;
    cam.lookFrom = Point(13, 2, 3);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void quads()
{
    HittableList world;

    // Materials
    auto leftRed = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto backGreen = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto rightBlue = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upperOrange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lowerTeal = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<Quad>(Point(-3, -2, 5), glm::dvec3(0, 0, -4), glm::dvec3(0, 4, 0), leftRed));
    world.add(make_shared<Quad>(Point(-2, -2, 0), glm::dvec3(4, 0, 0), glm::dvec3(0, 4, 0), backGreen));
    world.add(make_shared<Quad>(Point(3, -2, 1), glm::dvec3(0, 0, 4), glm::dvec3(0, 4, 0), rightBlue));
    world.add(make_shared<Quad>(Point(-2, 3, 1), glm::dvec3(4, 0, 0), glm::dvec3(0, 0, 4), upperOrange));
    world.add(make_shared<Quad>(Point(-2, -3, 5), glm::dvec3(4, 0, 0), glm::dvec3(0, 0, -4), lowerTeal));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 80;
    cam.lookFrom = Point(0, 0, 9);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void primitives()
{
    HittableList world;

    // Materials
    auto leftRed = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto backGreen = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto rightBlue = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upperOrange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lowerTeal = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Primitives
    // world.add(make_shared<Circle>(Point(-3, -0.5, 4), glm::dvec3(0, 0, -1.5), glm::dvec3(0, 1.5, 0), leftRed, 1));
    // world.add(make_shared<Triangle>(Point(0, -2, 0), glm::dvec3(3, 0, 0), glm::dvec3(0, 3, 0), backGreen));
    // world.add(make_shared<Triangle>(Point(0, -2, 0), glm::dvec3(-3, 0, 0), glm::dvec3(0, 3, 0), backGreen));
    // world.add(make_shared<Quad>(Point(4, -2, 1), glm::dvec3(0, 0, 3), glm::dvec3(0, 3, 0), rightBlue));
    // world.add(make_shared<Annuli>(Point(0, 3, 1), glm::dvec3(2, 0, 0), glm::dvec3(0, 0, 2), upperOrange, 0.8, 1.5));
    // world.add(make_shared<Ellipse>(Point(0, -3, 5), glm::dvec3(2, 0, 0), glm::dvec3(0, 0, -2), lowerTeal, 0.5, 1.5));
    // world.add(make_shared<Parabola>(Point(0, -2, 0), glm::dvec3(-3, 0, 0), glm::dvec3(0, 3, 0), backGreen, -0.5, false));
    // world.add(make_shared<Parabola>(Point(0, -2, 0), glm::dvec3(-3, 0, 0), glm::dvec3(0, 3, 0), backGreen, 0.5, false));
    world.add(make_shared<Hyperbola>(Point(0, -2, 0), glm::dvec3(-3, 0, 0), glm::dvec3(0, 3, 0), backGreen, 2, 1.5));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.useBackground = false;

    cam.verticalFov = 80;
    cam.lookFrom = Point(0, 0, 9);
    cam.lookAt = Point(0, 0, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void simpleLight()
{
    HittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(make_shared<Sphere>(Point(0, 7, 0), 2, difflight));
    world.add(make_shared<Quad>(Point(3, 1, -2), glm::dvec3(2, 0, 0), glm::dvec3(0, 2, 0), difflight));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Color(0, 0, 0);

    cam.verticalFov = 20;
    cam.lookFrom = Point(26, 3, 6);
    cam.lookAt = Point(0, 2, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void cornellBox()
{
    HittableList world;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point(555, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point(343, 554, 332), glm::dvec3(-130, 0, 0), glm::dvec3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(555, 0, 0), glm::dvec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(555, 555, 555), glm::dvec3(-555, 0, 0), glm::dvec3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point(0, 0, 555), glm::dvec3(555, 0, 0), glm::dvec3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = box(Point(0,0,0), Point(165,330,165), white);
    box1 = make_shared<RotateZ>(box1, 15);
    box1 = make_shared<Translate>(box1, glm::dvec3(265,0,295));
    world.add(box1);

    shared_ptr<Hittable> box2 = box(Point(0,0,0), Point(165,165,165), white);
    box2 = make_shared<RotateZ>(box2, -18);
    box2 = make_shared<Translate>(box2, glm::dvec3(130,0,65));
    world.add(box2);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = Color(0, 0, 0);

    cam.verticalFov = 40;
    cam.lookFrom = Point(278, 278, -800);
    cam.lookAt = Point(278, 278, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void cornellSmoke()
{
    HittableList world;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    world.add(make_shared<Quad>(Point(555, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point(113, 554, 127), glm::dvec3(330, 0, 0), glm::dvec3(0, 0, 305), light));
    world.add(make_shared<Quad>(Point(0, 555, 0), glm::dvec3(555, 0, 0), glm::dvec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(555, 0, 0), glm::dvec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(0, 0, 555), glm::dvec3(555, 0, 0), glm::dvec3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = box(Point(0, 0, 0), Point(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, glm::dvec3(265, 0, 295));

    shared_ptr<Hittable> box2 = box(Point(0, 0, 0), Point(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, glm::dvec3(130, 0, 65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = Color(0, 0, 0);

    cam.verticalFov = 40;
    cam.lookFrom = Point(278, 278, -800);
    cam.lookAt = Point(278, 278, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void finalScene(int imageWidth, int samplesPerPixel, int maxDepth)
{
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxesPerSide = 20;
    for (int i = 0; i < boxesPerSide; i++)
    {
        for (int j = 0; j < boxesPerSide; j++)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDoubleGen(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(Point(x0, y0, z0), Point(x1, y1, z1), ground));
        }
    }

    HittableList world;

    world.add(make_shared<BvhNode>(boxes1));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(make_shared<Quad>(Point(123, 554, 147), glm::dvec3(300, 0, 0), glm::dvec3(0, 0, 265), light));

    auto center1 = Point(400, 400, 200);
    auto center2 = center1 + glm::dvec3(30, 0, 0);
    auto sphereMaterial = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphereMaterial));

    world.add(make_shared<Sphere>(Point(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.add(make_shared<Sphere>(Point(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(Point(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("./image/earthmap.jpg"));
    world.add(make_shared<Sphere>(Point(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(make_shared<Sphere>(randomVectorGen(0, 165), 10, white));
    }

    world.add(make_shared<Translate>(make_shared<RotateY>(make_shared<BvhNode>(boxes2), 15), glm::dvec3(-100, 270, 395)));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = imageWidth;
    cam.samplesPerPixel = samplesPerPixel;
    cam.maxDepth = maxDepth;
    cam.background = Color(0, 0, 0);

    cam.verticalFov = 40;
    cam.lookFrom = Point(478, 278, -600);
    cam.lookAt = Point(278, 278, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void test()
{
    HittableList world;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point(555, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point(343, 554, 332), glm::dvec3(-130, 0, 0), glm::dvec3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point(0, 0, 0), glm::dvec3(555, 0, 0), glm::dvec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(555, 555, 555), glm::dvec3(-555, 0, 0), glm::dvec3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point(0, 0, 555), glm::dvec3(555, 0, 0), glm::dvec3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = box(Point(0,0,0), Point(165,330,165), white);
    box1 = make_shared<RotateZ>(box1, 15);
    box1 = make_shared<Translate>(box1, glm::dvec3(265,0,295));
    world.add(box1);

    shared_ptr<Hittable> box2 = box(Point(0,0,0), Point(165,165,165), white);
    box2 = make_shared<RotateZ>(box2, -18);
    box2 = make_shared<Translate>(box2, glm::dvec3(130,0,65));
    world.add(box2);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.background = Color(0, 0, 0);

    cam.verticalFov = 40;
    cam.lookFrom = Point(278, 278, -800);
    cam.lookAt = Point(278, 278, 0);
    cam.vUp = glm::dvec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render(world);
}