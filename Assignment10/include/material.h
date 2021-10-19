/*
 * @Description: Virtual Material to be extended
 * @Author: LvBujing
 * @Date: 2020-12-28 10:26:08
 * @LastEditTime: 2021-10-17 21:09:33
 * @LastEditors: LvBujing
 */
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include <GL/gl.h>
class RayTracer;
// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color, const Vec3f &r_color, 
    const Vec3f &t_color, const float &ind_fraction) { diffuseColor = d_color; 
      reflectiveColor = r_color; transparentColor = t_color; indexOfRefraction = ind_fraction; }
  Material(){}
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor(const Vec3f &pos) const { return diffuseColor; }
  virtual Vec3f getReflectiveColor(const Vec3f &pos)const{ return reflectiveColor; }
  virtual Vec3f getTransparentColor(const Vec3f &pos)const{ return transparentColor; }
  virtual float getIndexOfRefraction(const Vec3f &pos)const{ return indexOfRefraction; }

  // return shaded color
  virtual Vec3f Shade
    (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor, const float &distance, RayTracer* raytracer) = 0;

  virtual void glSetMaterial(void) const = 0;
  virtual void print() { 
    cout<<"#Material#"<<endl;
    cout<<"Diffusecolor: "<<diffuseColor<<endl;
    cout<<"Reflectivecolor: "<<reflectiveColor<<endl;
    cout<<"Transparentcolor: "<<transparentColor<<endl;
    cout<<"IndexOfrefraction: "<<indexOfRefraction<<endl;
  }

protected:

  // REPRESENTATION
  Vec3f diffuseColor,reflectiveColor,transparentColor;
  float indexOfRefraction;
};

// ====================================================================
// ====================================================================

#endif
