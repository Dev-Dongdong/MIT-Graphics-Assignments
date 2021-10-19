/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 19:55:16
 * @LastEditTime: 2020-12-26 17:13:41
 * @LastEditors: LvBujing
 */
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"

class Triangle : public Object3D{

private:
    Vec3f v0,v1,v2;

public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *mat) : v0(a),v1(b),v2(c){ m=mat; }
    ~Triangle(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin);
};

bool Triangle::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f O = r.getOrigin();
    Vec3f D = r.getDirection();

    Vec3f E1 = v1 - v0;
    Vec3f E2 = v2 - v0;
    Vec3f S = O - v0;
    Vec3f S1,S2;
    Vec3f::Cross3(S1,D,E2);
    Vec3f::Cross3(S2,S,E1);

    float denom = 1.0f / S1.Dot3(E1);
    float t = S2.Dot3(E2) * denom;
    float b = S1.Dot3(S) * denom;
    float c = S2.Dot3(D) * denom;
    float a = 1 - b - c;
    
    if(a < 0 || a > 1)return false;
    if(b < 0 || b > 1)return false;
    if(c < 0 || c > 1)return false;

    if(t < tmin || t > h.getT())return false;

    Vec3f normal;
    Vec3f::Cross3(normal,E1,E2);
    normal.Normalize();
    h.set(t,m,normal,r);
    //cout<<"hit!"<<endl;
    return true;
}

#endif