/*
 * @Description: Class of a Sphere
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-30 19:57:30
 * @LastEditors: LvBujing
 */
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"

#define EPSILON 0.0001f
#define M_PI 3.14159265358979323846

float thetaStep = 0;
float phiStep = 0;
bool isGouraud = false;
class Sphere : public Object3D{
    
private:
    Vec3f center;
    float radius;
public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Sphere(const Vec3f &c, const float &r, Material *mat):Object3D(mat){center = c; radius = r; }
    ~Sphere(){}

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        
        Vec3f dir = r.getDirection();
        Vec3f origin = r.getOrigin();
        Vec3f L = origin - center;

        float a = dir.Dot3(dir);
        float b = 2 * dir.Dot3(L);
        float c = L.Dot3(L) - radius * radius;

        float delta = b * b - 4 * a * c;

        if(delta < 0)return false;
        
        float t0 = (-b - sqrt(delta)) / (2 * a);
        float t1 = - b / a - t0;    //!!! used to calculate t1 with [t1 = c / (a * t0)], this can cause fatal error while t0 == 0

        
        if(t0 > t1)swap(t0,t1);
        
        //only the nearest one between tmin and h.getT() will account.
        if(t1 < tmin || t0 > h.getT())return false;
        if(t0 < tmin && t1 > h.getT())return false;

        if(t0 < tmin && t1 > tmin)t0 = t1;
        
        //update the hit information
        Vec3f normal = (r.getOrigin() + t0 * r.getDirection()) - center;
        normal.Normalize();
        h.set(t0,m,normal,r);
        return true;
        
    }
    void paint(void);
    Vec3f sphereCordToWorld(const float &phi, const float &theta);
};
Vec3f Sphere::sphereCordToWorld(const float &phi, const float &theta){
    return Vec3f(radius * cos(phi) * cos(theta),
                                    radius * sin(phi),
                                     radius * cos(phi) * sin(theta));
}
void Sphere::paint(void){
    m->glSetMaterial();

    glBegin(GL_QUADS);
    float deltaPhi = M_PI / phiStep;
    float deltaTheta = 2 * M_PI / thetaStep;
        for(float iPhi = - M_PI / 2; iPhi < M_PI / 2; iPhi += deltaPhi)   //  loop around all the angles in a ball.
            for(float iTheta = 0; iTheta < 2 * M_PI; iTheta += deltaTheta){
                Vec3f p0 = sphereCordToWorld(iPhi,iTheta);  // left up
                Vec3f p1 = sphereCordToWorld(iPhi,iTheta + deltaTheta); // right up
                Vec3f p2 = sphereCordToWorld(iPhi + deltaPhi,iTheta + deltaTheta);  // right down
                Vec3f p3 = sphereCordToWorld(iPhi + deltaPhi,iTheta);   // left down

                Vec3f quadNormal = p0 + p2;
                quadNormal.Normalize();
                
                if(isGouraud) { // set normal for each vertex
                    glNormal3f(p0.x(),p0.y(),p0.z());
                    p0 += center;
                    glVertex3f(p0.x(),p0.y(),p0.z());

                    glNormal3f(p1.x(),p1.y(),p1.z());
                    p1 += center;
                    glVertex3f(p1.x(),p1.y(),p1.z());

                    glNormal3f(p2.x(),p2.y(),p2.z());
                    p2 += center;
                    glVertex3f(p2.x(),p2.y(),p2.z());

                    glNormal3f(p3.x(),p3.y(),p3.z());
                    p3 += center;
                    glVertex3f(p3.x(),p3.y(),p3.z());
                } else {    //just set normal for this little plane
                    p0 += center;p1 += center;p2 += center;p3 += center;
                    glNormal3f(quadNormal.x(),quadNormal.y(),quadNormal.z());
                    glVertex3f(p0.x(),p0.y(),p0.z());
                    glVertex3f(p1.x(),p1.y(),p1.z());
                    glVertex3f(p2.x(),p2.y(),p2.z());
                    glVertex3f(p3.x(),p3.y(),p3.z());
                } 
                
            }
    glEnd();
}
#endif