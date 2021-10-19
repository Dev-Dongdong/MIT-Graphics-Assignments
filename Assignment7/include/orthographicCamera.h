/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-24 12:41:09
 * @LastEditTime: 2021-03-02 16:44:14
 * @LastEditors: LvBujing
 */

#ifndef _ORTHOGRAPHICCAMERA_H_
#define _ORTHOGRAPHICCAMERA_H_

#include "camera.h"

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

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);

    virtual void dollyCamera(float dist);   //scale the camera
    virtual void truckCamera(float dx, float dy);   // translate the camera
    virtual void rotateCamera(float rx, float ry);  // rotate the camera
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
    // calculate the horizontal and screenUp vector used for runtime ray generation
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    //Vec3f base = center - (size * screenUp) * 0.5f - (size * horizontal) * 0.5f;
    Vec3f offset = (size * horizontal) * (point.x() - 0.5f) + (size * screenUp) * (point.y() - 0.5f);
    Ray ray(center + offset,direction);
    return ray;
}

// Orthographic Camera will record the nearest pixel along the direction, no matter how close the object is
float OrthographicCamera::getTMin() const{
    return -INFINITY;
}

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================
void OrthographicCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w > h)    // set the length of longer side with size.
    glOrtho(-size/2.0, size/2.0, -size*(float)h/(float)w/2.0, size*(float)h/(float)w/2.0, 0.5, 40.0);
  else
    glOrtho(-size*(float)w/(float)h/2.0, size*(float)w/(float)h/2.0, -size/2.0, size/2.0, 0.5, 40.0);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================
void OrthographicCamera::glPlaceCamera(void)
{
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist)
{
  center += direction*dist;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy)
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
// ATTENTION: You can not rotate the camera around the direction vector
// The rotate center is the origin camera center
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry)
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
  
  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
  direction.Normalize();

}
#endif