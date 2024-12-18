#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hittable.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &rIn, const HitRecord &rec, color &attenuation, Ray &scattered)
        const { return false; }
};

class Lambertian : public Material
{
public:
    Lambertian(const color &albedo) : albedo(albedo) {}

    bool scatter(const Ray &rIn, const HitRecord &rec, color &attenuation, Ray &scattered) const override
    {
        auto scatterDirection = rec.normal + randomUnitVector();

        // Catch degenerate scatter direction
        if (nearZero(scatterDirection))
            scatterDirection = rec.normal;

        scattered = Ray(rec.p, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class Metal : public Material
{
public:
    Metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray &rIn, const HitRecord &rec, color &attenuation, Ray &scattered) const override
    {
        glm::dvec3 reflected = glm::reflect(rIn.direction(), rec.normal);
        // fuzzy reflection
        // reflected = glm::normalize(reflected) + (fuzz + randomUnitVector());
        // non-fuzzy reflection
        reflected = glm::normalize(reflected);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return glm::dot(scattered.direction(), rec.normal) > 0;
    }

private:
    color albedo;
    double fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool scatter(const Ray &rIn, const HitRecord &rec, color &attenuation, Ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        glm::dvec3 unitDirection = glm::normalize(rIn.direction());

        double cosTheta = std::fmin(glm::dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0;
        glm::dvec3 direction;

        if (cannotRefract || reflectance(cosTheta, ri) > randomDouble())
            direction = glm::reflect(unitDirection, rec.normal);
        else
            direction = glm::refract(unitDirection, rec.normal, ri);

        scattered = Ray(rec.p, direction);

        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
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

#endif //_MATERIAL_H_