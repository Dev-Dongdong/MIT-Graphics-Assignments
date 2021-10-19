/*
 * @Description: Class of a Sphere
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-25 11:31:55
 * @LastEditors: LvBujing
 */
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object3D.h"

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
        
        if(t1 < tmin)return false;

        
        
        if(t0 > tmin){
            if(t0 < h.getT())h.set(t0,m,r);
        }else{
            if(t1 < h.getT())h.set(t1,m,r);
        }
        
        return true;
    }
};
#endif