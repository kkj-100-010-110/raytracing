#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "common.h"

class Perlin
{
public:
    Perlin()
    {
        for (int i = 0; i < pointCount; i++)
            randvec[i] = randomUnitVector();

        perlinGeneratePerm(permX);
        perlinGeneratePerm(permY);
        perlinGeneratePerm(permZ);
    }

    double noise(const glm::dvec4& p) const
    {
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);

        auto i = int(std::floor(p.x));
        auto j = int(std::floor(p.y));
        auto k = int(std::floor(p.z));
        glm::dvec4 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = randvec[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];

        return perlinInterp(c, u, v, w);
    }

    double turb(const glm::dvec4& p, int depth) const
    {
        auto accum = 0.0;
        auto tempP = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int pointCount = 256;
    glm::dvec4 randvec[pointCount];
    int permX[pointCount];
    int permY[pointCount];
    int permZ[pointCount];

    static void perlinGeneratePerm(int *p)
    {
        for (int i = 0; i < pointCount; i++)
            p[i] = i;
        permute(p, pointCount);
    }

    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = randomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlinInterp(const glm::dvec4 c[2][2][2], double u, double v, double w)
    {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    glm::dvec4 weightV(u - i, v - j, w - k, 0);
                    accum += (i * uu + (1 - i) * (1 - uu))
                           * (j * vv + (1 - j) * (1 - vv))
                           * (k * ww + (1 - k) * (1 - ww))
                           * glm::dot(c[i][j][k], weightV);
                }

        return accum;
    }
};

#endif//_PERLIN_H_