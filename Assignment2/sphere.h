/*
 * @Description: Class of a Sphere
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-27 16:04:19
 * @LastEditors: LvBujing
 */
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"

#define EPSILON 0.0001f

class Sphere : public Object3D{
    
private:
    Vec3f center;
    float radius;
public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Sphere(const Vec3f &c, const float &r, Material *mat):Object3D(mat){center = c; radius = r; }
    ~Sphere(){}

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        
        Vec3f dir = r.getDirection();
        Vec3f origin = r.getOrigin();
        Vec3f L = origin - center;

        float a = dir.Dot3(dir);
        float b = 2 * dir.Dot3(L);
        float c = L.Dot3(L) - radius * radius;

        float delta = b * b - 4 * a * c;

        if(delta < 0)return false;
        
        float t0 = (-b - sqrt(delta)) / (2 * a);
        float t1 = - b / a - t0;    //!!! used to calculate t1 with [t1 = c / (a * t0)], this can cause fatal error while t0 == 0

        
        if(t0 > t1)swap(t0,t1);
        
        //only the nearest one between tmin and h.getT() will account.
        if(t1 < tmin || t0 > h.getT())return false;
        if(t0 < tmin && t1 > h.getT())return false;

        if(t0 < tmin && t1 > tmin)t0 = t1;
        
        //update the hit information
        Vec3f normal = (r.getOrigin() + t0 * r.getDirection()) - center;
        normal.Normalize();
        h.set(t0,m,normal,r);
        return true;
        
    }
};
#endif