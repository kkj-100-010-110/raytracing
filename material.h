#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hittable.h"
#include "texture.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual Color emitted(double u, double v, const Point& p) const { return Color(0, 0, 0); }

    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const { return false; }
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& albedo) : tex(make_shared<SolidColor>(albedo)) {}
    Lambertian(shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        auto scatterDirection = rec.normal + randomUnitVectorGen();

        // Catch degenerate scatter direction
        if (nearZero(scatterDirection)) scatterDirection = rec.normal;

        scattered = Ray(rec.p, scatterDirection, rayIn.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<Texture> tex;
};

class Metal : public Material
{
public:
    Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        glm::dvec3 reflected = reflect(rayIn.direction(), rec.normal);
        reflected = glm::normalize(reflected) + (fuzz * randomUnitVectorGen());
        scattered = Ray(rec.p, reflected, rayIn.time());
        attenuation = albedo;
        return (glm::dot(scattered.direction(), rec.normal) > 0);
    }

private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        glm::dvec3 unitDirection = glm::normalize(rayIn.direction());
        double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannot_refract = ri * sinTheta > 1.0;
        glm::dvec3 direction;

        if (cannot_refract || reflectance(cosTheta, ri) > randomDoubleGen())
            direction = reflect(unitDirection, rec.normal);
        else
            direction = refract(unitDirection, rec.normal, ri);

        scattered = Ray(rec.p, direction, rayIn.time());
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the Material's refractive index over
    // the refractive index of the enclosing media
    double refractionIndex;

    static double reflectance(double cosine, double refractionIndex)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(shared_ptr<Texture> tex) : tex(tex) {}
    DiffuseLight(const Color& emit) : tex(make_shared<SolidColor>(emit)) {}

    Color emitted(double u, double v, const Point& p) const override { return tex->value(u, v, p); }

private:
    shared_ptr<Texture> tex;
};

class Isotropic : public Material
{
public:
    Isotropic(const Color& albedo) : tex(make_shared<SolidColor>(albedo)) {}
    Isotropic(shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
    {
        scattered = Ray(rec.p, randomUnitVectorGen(), rayIn.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<Texture> tex;
};

#endif//_MATERIAL_H_