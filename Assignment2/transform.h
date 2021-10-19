/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 19:55:59
 * @LastEditTime: 2021-09-29 10:16:04
 * @LastEditors: LvBujing
 */
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"

class Transform : public Object3D{

private:
    Matrix matrix;
    Object3D* obj;
public:
    Transform(const Matrix &_matrix, Object3D* _obj): matrix(_matrix),obj(_obj){}
    ~Transform(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin);
};

// This function maintain all the transform towards object.(Actually singly transform the ray)
bool Transform::intersect(const Ray &r, Hit &h, float tmin){
    //before enter this function, you should assume that all the input variables(e.g. r,h,tmin)
    //are in the world space
    Vec4f homoOrigion = Vec4f(r.getOrigin(),1);
    Vec4f homoDir = Vec4f(r.getDirection(),0);

    Matrix tempMatrix = matrix; // Do not bother the origin matrix.

    tempMatrix.Inverse();   // transform the object using "matrix" == transform the ray using "inverse matrix".
    tempMatrix.Transform(homoDir);  //transform ray-dir to object space
    tempMatrix.Transform(homoOrigion);  //transform ray-origin to object space
    
    Vec3f newOrigin = Vec3f(homoOrigion.x(),homoOrigion.y(),homoOrigion.z());
    Vec3f newDir = Vec3f(homoDir.x(),homoDir.y(),homoDir.z());
    
    float scaleCoefficient = newDir.Length();   //if you normalize the newDir, after intersection check you will need scaleCoefficient to transform t back.
    //if(newDir.Dot3(r.getDirection()) < 0)scaleCoefficient = - scaleCoefficient; //This make no sense
    
    newDir.Normalize();
    Ray ray = Ray(newOrigin,newDir);    //a temp ray in object space

    Hit tempH;  // a temp hit to store intersection information in object space.
    if(!obj->intersect(ray,tempH,tmin))return false;   

    float newT = tempH.getT() / scaleCoefficient;   //the true t
    if(newT > h.getT())return false;    //Use true t to compare rather than tempH.getT()   <--- this one is in object space while h.getT() is in world space

    Vec4f objectNormal = Vec4f(tempH.getNormal(),0);
    
    // handle the normal, construct a transpose of inverse matrix.
    tempMatrix.Transpose();
    tempMatrix.Transform(objectNormal);
    Vec3f worldNormal = Vec3f(objectNormal.x(),objectNormal.y(),objectNormal.z());
    worldNormal.Normalize();
    
    h.set(newT,tempH.getMaterial(),worldNormal,r);
    return true;
}

#endif