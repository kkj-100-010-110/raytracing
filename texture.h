#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "perlin.h"
#include "rtw_stb_image.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point& p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(const Color& albedo) : albedo(albedo) {}

    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

    Color value(double u, double v, const Point& p) const override { return albedo; }

private:
    Color albedo;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) : invScale(1.0 / scale), even(even), odd(odd) {}

    CheckerTexture(double scale, const Color& c1, const Color& c2)
        : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2))
    {}

    Color value(double u, double v, const Point& p) const override
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
    ImageTexture(const char* filename) : image(filename) {}

    Color value(double u, double v, const Point& p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0)
            return Color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v); // Flip V to image coordinates

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto colorScale = 1.0 / 255.0;
        return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
    }

private:
    RtwImage image;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(double scale) : scale(scale) {}

    Color value(double u, double v, const Point& p) const override
    {
        return Color(.5, .5, .5) * (1 + std::sin(scale * p.z + 10 * noise.turb(p, 7)));
    }

private:
    Perlin noise;
    double scale;
};

#endif//_TEXTURE_H_