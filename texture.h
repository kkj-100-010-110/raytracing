#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "common.h"
#include "rtw_stb_image.h"
#include "perlin.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual color value(double u, double v, const glm::dvec4& p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(const color& albedo) : albedo(albedo) {}

    SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue, 0.0)) {}

    SolidColor(double red, double green, double blue, double alpha = 0.0) : SolidColor(color(red, green, blue, alpha)) {}

    color value(double u, double v, const glm::dvec4& p) const override { return albedo; }

private:
    color albedo;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
        : invScale(1.0 / scale), even(even), odd(odd) {}

    CheckerTexture(double scale, const color& c1, const color& c2)
        : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

    color value(double u, double v, const glm::dvec4& p) const override
    {
        auto xInteger = int(std::floor(invScale * p.x));
        auto yInteger = int(std::floor(invScale * p.y));
        auto zInteger = int(std::floor(invScale * p.z));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double invScale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class ImageTexture : public Texture
{
public:
    ImageTexture(const char *filename) : image(filename) {}

    color value(double u, double v, const glm::dvec4&p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0, 1, 1, 0);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v); // Flip V to image coordinates

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2], 0.0);
    }

private:
    RtwImage image;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture() {}
    NoiseTexture(double scale) : scale(scale) {}


    color value(double u, double v, const glm::dvec4& p) const override
    {
        // return color(1, 1, 1, 0) * 0.5 * (1.0 + noise.noise(scale * p));
        // return color(1, 1, 1, 0) * noise.turb(p, 7);
        return color(.5, .5, .5, 0) * (1 + std::sin(scale * p.z + 10 * noise.turb(p, 7)));
    }

private:
    Perlin noise;
    double scale;
};

#endif//_TEXTURE_H_