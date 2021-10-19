/*
 * @Description: The PhongMaterial used to produce highlight
 * @Author: LvBujing
 * @Date: 2020-12-28 15:34:16
 * @LastEditTime: 2021-10-18 18:27:09
 * @LastEditors: LvBujing
 */
#ifndef _PHONGMATERIAL_H_
#define _PHONGMATERIAL_H_

#include "vectors.h"
#include "material.h"
#include "glCanvas.h"


#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class PhongMaterial : public Material{

public:

  // CONSTRUCTORS & DESTRUCTOR
  PhongMaterial(const Vec3f &diffuseColor, 
     const Vec3f &specularColor, float exponent,
     const Vec3f &reflectiveColor, const Vec3f &transparentColor,
     float indexOfRefraction): Material(diffuseColor,reflectiveColor,transparentColor,indexOfRefraction),
     specularColor(specularColor), exponent(exponent) {}
  ~PhongMaterial() {}

  // The disance attenuation already be calculated in lightcolor while the light is pointlight
  virtual Vec3f Shade
    (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor, const float &distance, RayTracer* raytracer) {
       // Diffuse part
      float NdotL = hit.getNormal().Dot3(dirToLight);
      if(NdotL < 0)NdotL = 0; //Clamp. Not deal with the back surface
      Vec3f diffuse = NdotL * lightColor * diffuseColor;

      // Specular part
      Vec3f specular(0,0,0);
      Vec3f h = dirToLight - ray.getDirection();
      h.Normalize();
      float HdotN = h.Dot3(hit.getNormal());
      if(HdotN >= 0)specular = specularColor * powf(HdotN,exponent) * lightColor;
      //else specular = specularColor * HdotN * lightColor;
      
      return diffuse + specular;
     }

  // ====================================================================
  // Set the OpenGL parameters to render with the given material
  // attributes.
  // ====================================================================
  virtual void glSetMaterial(void) const{

  GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat specular[4] = {
    getSpecularColor().r(),
    getSpecularColor().g(),
    getSpecularColor().b(),
    1.0};
  GLfloat diffuse[4] = {
    diffuseColor.r(),
    diffuseColor.g(),
    diffuseColor.b(),
    1.0};
  
  // NOTE: GL uses the Blinn Torrance version of Phong...      
  float glexponent = exponent;
  if (glexponent < 0) glexponent = 0;
  if (glexponent > 128) glexponent = 128;
  
#if !SPECULAR_FIX 
    
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  
#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight 
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    
  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
  }
  
  // ========
  // ACCESSOR
  Vec3f getSpecularColor()const{ return specularColor; }
  
  
protected:
  Vec3f specularColor;
  float exponent;
};





#endif
