/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 20:12:35
 * @LastEditTime: 2020-12-26 17:25:31
 * @LastEditors: LvBujing
 */

#ifndef _PERSPECTIVECAMERA_H_
#define _PERSPECTIVECAMERA_H_

#include "camera.h"

class PerspectiveCamera : public Camera {
    
private:
    Vec3f center;
    Vec3f up,direction,horizontal;
    float fov;
public:
    PerspectiveCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &fov);
    ~PerspectiveCamera(){}
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const;
};

PerspectiveCamera::PerspectiveCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &_fov){
   
    direction = dir;
    direction.Normalize();
    center = c;
    fov = _fov;
    Vec3f dir_sub = direction.Dot3(_up) * direction;
    up = _up - dir_sub;
    up.Normalize();
    Vec3f::Cross3(horizontal,direction,up);
    horizontal.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point){
    float virtualD = 1;
    Vec3f virtualCenter = center + virtualD * direction;
    float size = 2 * virtualD * tan(0.5f * fov);
    Vec3f base = virtualCenter - (size * up) * 0.5f - (size * horizontal) * 0.5f;
    Vec3f offset = (size * horizontal) * point.x() + (size * up) * point.y();
    Vec3f dir = base + offset - center;
    dir.Normalize();
    Ray ray(center,dir);
    return ray;
}

float PerspectiveCamera::getTMin() const{
    return 0.0f;
}
#endif