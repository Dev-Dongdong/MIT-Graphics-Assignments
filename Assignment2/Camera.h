/*
 * @Description: The base class of Camera
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-24 12:49:23
 * @LastEditors: LvBujing
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"
#include "ray.h"

class Camera {
    
private:
    Material* m;
public: 

    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Camera(){}
    ~Camera(){}

    // =========
    // FUNCTIONS
    // =========
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

#endif