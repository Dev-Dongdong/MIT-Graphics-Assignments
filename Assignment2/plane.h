/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 19:53:48
 * @LastEditTime: 2020-12-26 16:13:28
 * @LastEditors: LvBujing
 */

#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include "float.h"

// Plane define a infinity size plane.
class Plane : Object3D{
    
private:
    Vec3f normal;
    // offset describe the signed distance between plane and space origin
    float offset;

public:
    Plane(const Vec3f &n, const float &_offset, Material* mat){normal = n; offset = _offset; m = mat; }
    ~Plane(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin);
};

bool Plane::intersect(const Ray &r, Hit &h, float tmin){
    
    Vec3f origin = r.getOrigin();
    Vec3f dir = r.getDirection();

    float OdotN = origin.Dot3(normal);  //
    float DdotN = dir.Dot3(normal);

    if(abs(offset - OdotN) < FLT_EPSILON){  // the ray origin is in this plane.[Intersected]
        h.set(0,m,normal,r);
        return true;
    }
    if(abs(DdotN) < FLT_EPSILON)return false;   // the ray origin is not in this plane, but normal is perpendicular to dir.[Not Intersected]

    float t = (offset - OdotN) / DdotN;
    //cout<<tmin<<" "<<t<<" "<<h.getT()<<endl;
    if(t < tmin || t > h.getT())return false;    // if the intersection's t is between tmin and h.getT()
    else h.set(t,m,normal,r);                    // the ray must intersect with this plane[Intersected]
    return true;
}




#endif  