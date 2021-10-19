/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2021-10-18 15:16:12
 * @LastEditTime: 2021-10-19 20:32:34
 * @LastEditors: LvBujing
 */
#include "SSSMaterial.h"
#include "raytracer.h"

Vec3f SSSMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
        const Vec3f &lightColor, const float &distance, RayTracer* raytracer) { // We shade the SSS material
        // First we calculate the origin color using Blinn-Phong shading
        Vec3f originColor = PhongMaterial::Shade(ray,hit,dirToLight,lightColor,distance,raytracer);
        Vec3f I(0,0,0);
        Ray r(hit.getIntersectionPoint(), dirToLight);
        Hit sssInter;

        Vec3f V = ray.getDirection(); // The vector from viewer to shadering point
        V.Negate(); // the view vector
        Vec3f H = dirToLight + hit.getNormal() * L2NDistortion; // get the dir after distortion
        H.Normalize(); 
        H.Negate(); // we will use -H later
        
        float VdotH = pow(std::max(0.f, std::min(1.f, V.Dot3(H))), power) * scale;
        float thickness = 1.f;
        if (raytracer->isIntersect(r, sssInter, SSSEPSILON)) {
            thickness = sssInter.getT();
            thickness = exp(-C*thickness);
        }
        I = attenuation * (Vec3f(VdotH, VdotH, VdotH) + ambient) * thickness;
        return originColor + lightColor * I;
    }