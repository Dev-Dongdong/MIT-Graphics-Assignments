/*
 * @Description: Trace the ray in a scene.
 * @Author: LvBujing
 * @Date: 2021-01-05 10:42:00
 * @LastEditTime: 2021-10-18 15:29:10
 * @LastEditors: LvBujing
 */
#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_
#include "scene_parser.h"
#include "rayTree.h"
#include "grid.h"
#include "matrix.h"
#include "group.h"
#include "light.h"
#include "raytracing_stats.h"


class RayTracer{
    
private:
    SceneParser *p;
    Grid *g;
    int max_bounces;
    float cutoff_weight;
    bool shadows,ifShadeBack;
public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    RayTracer(SceneParser *p, int max_bounces, float cutoff_weight, 
            bool shadows, bool ifShadeBack);
    ~RayTracer(){if(p!=NULL)delete p;}

    // ========
    // FUNCTION
    // Trace a ray in the scene and return colors
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &hit);
    bool rayCast(const Ray &r, Hit &h, float tmin);
    bool fastRayCast(const Ray &r, Hit &h, float tmin); // Found intersection with grid acceleration.
    Grid* getGrid(){ return g;}
    bool isIntersect(const Ray &r, Hit &h, float tmin); // Only return if the ray is blocked by scene and if yes return the distance t stored in h
};


#endif