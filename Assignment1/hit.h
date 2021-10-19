/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-23 20:03:40
 * @LastEditTime: 2020-12-25 10:05:10
 * @LastEditors: LvBujing
 */
#ifndef _HIT_H
#define _HIT_H

#include "vectors.h"
#include "ray.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { material = NULL; t = __FLT_MAX__; }
  Hit(float _t, Material *m) { 
    t = _t; material = m; }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    intersectionPoint = h.intersectionPoint; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  
  // MODIFIER
  void set(float _t, Material *m, const Ray &ray) {
    t = _t; material = m; 
    intersectionPoint = ray.pointAtParameter(t); }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f intersectionPoint;

};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <t:" << h.getT() <<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif
