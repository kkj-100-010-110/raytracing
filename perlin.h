#ifndef _PERLIN_H_
#define _PERLIN_H_

class Perlin
{
public:
    Perlin()
    {
        for (int i = 0; i < point_count; i++)
            randVec[i] = glm::normalize(randomVectorGen(-1, 1));

        perlinGeneratePerm(permX);
        perlinGeneratePerm(permY);
        perlinGeneratePerm(permZ);
    }

    double noise(const Point& p) const
    {
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);

        auto i = int(std::floor(p.x));
        auto j = int(std::floor(p.y));
        auto k = int(std::floor(p.z));
        glm::dvec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = randVec[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];

        return perlinInterp(c, u, v, w);
    }

    double turb(const Point& p, int depth) const
    {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int point_count = 256;
    glm::dvec3 randVec[point_count];
    int permX[point_count];
    int permY[point_count];
    int permZ[point_count];

    static void perlinGeneratePerm(int* p)
    {
        for (int i = 0; i < point_count; i++)
            p[i] = i;

        permute(p, point_count);
    }

    static void permute(int* p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = randomIntGen(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlinInterp(const glm::dvec3 c[2][2][2], double u, double v, double w)
    {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    glm::dvec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                             glm::dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};

#endif//_PERLIN_H_