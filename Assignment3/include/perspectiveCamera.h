/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 20:12:35
 * @LastEditTime: 2020-12-28 15:28:45
 * @LastEditors: LvBujing
 */

#ifndef _PERSPECTIVECAMERA_H_
#define _PERSPECTIVECAMERA_H_

#include "camera.h"
#include "GL/gl.h"
#include "GL/glu.h"


class PerspectiveCamera : public Camera {
    
private:
    Vec3f center;
    Vec3f up,direction,horizontal;
    float angle;
public:
    PerspectiveCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &angle);
    ~PerspectiveCamera(){}
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const;

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);

    virtual void dollyCamera(float dist);   //scale the camera
    virtual void truckCamera(float dx, float dy);   // translate the camera
    virtual void rotateCamera(float rx, float ry);  // rotate the camera
};

PerspectiveCamera::PerspectiveCamera(const Vec3f &c, const Vec3f dir, const Vec3f _up, const float &_angle){
   
    direction = dir;
    direction.Normalize();
    center = c;
    angle = _angle;
    Vec3f dir_sub = direction.Dot3(_up) * direction;
    up = _up - dir_sub;
    up.Normalize();
    Vec3f::Cross3(horizontal,direction,up);
    horizontal.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point){
    float virtualD = 1;
    Vec3f virtualCenter = center + virtualD * direction;
    float size = 2 * virtualD * tan(0.5f * angle);
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

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
  center += direction*dist;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  Vec3f screenUp;
  Vec3f::Cross3(screenUp, horizontal, direction);

  center += horizontal*dx + screenUp*dy;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  direction.Normalize();

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}
#endif