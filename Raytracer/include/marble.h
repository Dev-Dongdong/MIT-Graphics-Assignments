/*
 * @Description: A class of Marble material
 * @Author: LvBujing
 * @Date: 2021-02-25 15:12:01
 * @LastEditTime: 2021-03-01 16:19:14
 * @LastEditors: LvBujing
 */


#include "material.h"
#include "matrix.h"

class Marble : public Material {

private:
    Material *mat1,*mat2;
    Matrix *m;
    int octaves;
    float frequency, amplitude;
public:
    Marble(Matrix *_m, Material *_mat1, Material *_mat2, int _octaves, float _frequency, float _amplitude) 
        : mat1(_mat1), mat2(_mat2), m(_m), octaves(_octaves), frequency(_frequency), amplitude(_amplitude) { }
    ~Marble() { if (mat1 != NULL) delete mat1; if (mat2 != NULL) delete mat2; if (m != NULL) delete m; }
    void glSetMaterial(void) const { }
    Vec3f getDiffuseColor(const Vec3f &pos) const { 
        Vec3f tempPos = pos;
        m->Transform(tempPos);
        float N = 0, M;
        int step = 1;
        for (int i = 0; i < octaves; i++) {
            N += PerlinNoise::noise(step * tempPos.x(), step * tempPos.y(), step * tempPos.z()) / step;
            step *= 2;
        }
        M = sin(frequency * tempPos.x() + amplitude * N);
        M = (M + 1) / 2;
        return mat1->getDiffuseColor(pos) * (M) + mat2->getDiffuseColor(pos) * (1 - M);
    }


    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor, const float &distance) const {
        Vec3f pos = hit.getIntersectionPoint();
        m->Transform(pos);
        float N = 0, M;
        int step = 1;
        for (int i = 0; i < octaves; i++) {
            N += PerlinNoise::noise(step * pos.x(), step * pos.y(), step * pos.z()) / step;
            step *= 2;
        }
        M = sin(frequency * pos.x() + amplitude * N);
        M = (M + 1) / 2;
        return mat1->Shade(ray,hit,dirToLight,lightColor,distance) * (M) + mat2->Shade(ray,hit,dirToLight,lightColor,distance) * (1 - M);
    }
};

