/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-24 12:41:09
 * @LastEditTime: 2020-12-25 11:31:41
 * @LastEditors: LvBujing
 */

#ifndef _ORTHOGRAPHICCAMERA_H_
#define _ORTHOGRAPHICCAMERA_H_

#include "Camera.h"

class OrthographicCamera : public Camera {
    
private:
    Vec3f center;
    Vec3f up,direction,horizontal;
    float size;
public:
    OrthographicCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &s);
    ~OrthographicCamera(){}
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const;
};

OrthographicCamera::OrthographicCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &s){
   
    direction = dir;
    direction.Normalize();
    center = c;
    size = s;
    Vec3f dir_sub = direction.Dot3(_up) * direction;
    up = _up - dir_sub;
    up.Normalize();
    Vec3f::Cross3(horizontal,direction,up);
    horizontal.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point){
    Vec3f base = center - (size * up) * 0.5f - (size * horizontal) * 0.5f;
    Vec3f offset = (size * horizontal) * point.x() + (size * up) * point.y();
    Ray ray(base + offset,direction);
    return ray;
}

float OrthographicCamera::getTMin() const{
    return -__FLT_MAX__;
}
#endif