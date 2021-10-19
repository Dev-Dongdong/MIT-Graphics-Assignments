/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 19:55:59
 * @LastEditTime: 2021-01-06 15:37:00
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
    void paint(void);
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
    
    Vec3f newOrigin = Vec3f(homoOrigion.x()/homoOrigion.w(),homoOrigion.y()/homoOrigion.w(),homoOrigion.z()/homoOrigion.w());
    Vec3f newDir = Vec3f(homoDir.x(),homoDir.y(),homoDir.z());
    
    float scaleCoefficient = newDir.Length();   //if you normalize the newDir, after intersection check you will need scaleCoefficient to transform t back.
    //if(newDir.Dot3(r.getDirection()) < 0)scaleCoefficient = - scaleCoefficient;   //Do Not use this---will cause black line in the middle.
    
    newDir.Normalize();
    Ray ray = Ray(newOrigin,newDir);    //a temp ray in object space

    Hit tempH;  // a temp hit to store intersection information in object space.
    if(!obj->intersect(ray,tempH,tmin * scaleCoefficient))return false;   

    float newT = tempH.getT() / scaleCoefficient;   //the true t
    if(newT < tmin || newT > h.getT())return false;    //Use true t to compare rather than tempH.getT()   <--- this one is in object space while h.getT() is in world space

    Vec4f objectNormal = Vec4f(tempH.getNormal(),0);
    
    // handle the normal, construct a transpose of inverse matrix.
    tempMatrix.Transpose();
    tempMatrix.Transform(objectNormal);
    Vec3f worldNormal = Vec3f(objectNormal.x(),objectNormal.y(),objectNormal.z());
    worldNormal.Normalize();
    
    h.set(newT,tempH.getMaterial(),worldNormal,r);
    return true;
}

void Transform::paint(void){
    glPushMatrix();
        GLfloat *glMatrix = matrix.glGet();
        glMultMatrixf(glMatrix);
        delete [] glMatrix;
        obj->paint();   // this must call after delete glMatrix, otherwise you may not observe the bunny.
    glPopMatrix();
}
#endif