/*
 * @Description: The base class of Camera
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-28 15:16:05
 * @LastEditors: LvBujing
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"
#include "ray.h"
#include <GL/gl.h>
#include <GL/glu.h>

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

    
    virtual Ray generateRay(Vec2f point) = 0;   // generate a ray from origin to point in virtual plane
    virtual float getTMin() const = 0;  // get tmin of this camera

    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    
    virtual void dollyCamera(float dist) = 0;   //scale the camera
    virtual void truckCamera(float dx, float dy) = 0;   // translate the camera
    virtual void rotateCamera(float rx, float ry) = 0;  // rotate the camera
};

#endif