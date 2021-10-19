/*
 * @Description: A special Material for some SubSurfaceScattering Material, such as milk, skin, marble, jade, etc.
 * @Author: LvBujing
 * @Date: 2021-10-17 14:16:23
 * @LastEditTime: 2021-10-18 18:24:01
 * @LastEditors: LvBujing
 */
#ifndef _SSSMATERIAL_H_
#define _SSSMATERIAL_H_
#define SSSEPSILON 0.001f

#include "vectors.h"
#include "phongMaterial.h"
#include "hit.h"

class SSSMaterial : public PhongMaterial{

public:
    // CONSTRUCTOR & DESTRUCTOR
    SSSMaterial(const Vec3f &diffuseColor, 
     const Vec3f &specularColor, float exponent,
     const Vec3f &reflectiveColor, const Vec3f &transparentColor,
     float indexOfRefraction, float wrap, float C, float L2NDistortion, float power, 
     float scale, float attenuation, const Vec3f &ambient): PhongMaterial(diffuseColor,specularColor,exponent,
     reflectiveColor,transparentColor,indexOfRefraction), wrap(wrap), C(C), L2NDistortion(L2NDistortion), power(power), 
     scale(scale), attenuation(attenuation), ambient(ambient) {}
    ~SSSMaterial() {}
    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
        const Vec3f &lightColor, const float &distance, RayTracer* raytracer);
    virtual void glSetMaterial(void) {} // Do not need to render this material in opengl

private:
    float wrap; // wrap diffuse 
    float C; // coefficent to adjust thickness
    float L2NDistortion; // The coefficient to distort -L toward surface normal
    float power, scale; // The power and scale which will effect the -L toward surface normal
    float attenuation; // Controll efficient attenuation
    Vec3f ambient; // Subsurface Scattering Ambient, which always make sence
};
#endif
