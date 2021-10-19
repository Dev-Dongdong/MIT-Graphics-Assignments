/*
 * @Description: Class of a Sphere
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2021-02-25 16:06:37
 * @LastEditors: LvBujing
 */
#ifndef _SPHERE_H_
#define _SPHERE_H_


#include "object3d.h"
#include "grid.h"
#include "transform.h"
#define EPSILON 0.00001f
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
    Sphere(const Vec3f &c, const float &r, Material *mat):Object3D(mat){
        center = c; radius = r; 
        box = new BoundingBox(Vec3f(c.x()-r,c.y()-r,c.z()-r),Vec3f(c.x()+r,c.y()+r,c.z()+r));
    }
    ~Sphere(){}

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        //cout<<"now come into intersect."<<endl;
        RayTracingStats::IncrementNumIntersections();
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
        //cout<<normal<<endl;
        h.set(t0,m,normal,r);
        return true;
        
    }
    void insertIntoGrid(Grid *g, Matrix *m);
    void paint(void);
    Vec3f sphereCordToWorld(const float &phi, const float &theta);
};

void Sphere::insertIntoGrid(Grid *g, Matrix *m){
    BoundingBox* boxSaver = box;
    if(m != NULL){  // If with transform, the sphere should be perform as a cube, so explicitly call Object3D's insertIntoGrid func.
        //cout<<"Before: ";
        //box->Print();
        this->Object3D::insertIntoGrid(g,m);
        //cout<<"After: ";
        //box->Print();
    }
    Vec3f nVector = g->getNVector();
    Vec3f box_MinP = box->getMin();
    Vec3f box_MaxP = box->getMax();
    Vec3f minP = g->getBoundingBox()->getMin();
    Vec3f maxP = g->getBoundingBox()->getMax();
    float deltaX = (maxP.x() - minP.x()) / nVector.x();
    
    float deltaY = (maxP.y() - minP.y()) / nVector.y();
    float deltaZ = (maxP.z() - minP.z()) / nVector.z();
    float deltaL = sqrt(deltaX*deltaX*0.25f + deltaY*deltaY*0.25f + deltaZ*deltaZ*0.25f);
    int i_begin,j_begin,k_begin;
    int i_end,j_end,k_end;
    i_begin = (box_MinP.x() - minP.x()) / deltaX - EPSILON; j_begin = (box_MinP.y() - minP.y()) / deltaY - EPSILON; k_begin = (box_MinP.z() - minP.z()) / deltaZ - EPSILON;
    i_end = (box_MaxP.x() - minP.x()) / deltaX - EPSILON; j_end = (box_MaxP.y() - minP.y()) / deltaY - EPSILON; k_end = (box_MaxP.z() - minP.z()) / deltaZ - EPSILON;
    
    float x_pieces = (box_MaxP.x() - minP.x()) / deltaX;
    if(abs(x_pieces - (int)(x_pieces+EPSILON))< EPSILON && i_end + 1 < nVector.x())i_end++;
    
    float y_pieces = (box_MaxP.y() - minP.y()) / deltaY;
    if(abs(y_pieces - (int)(y_pieces+EPSILON))< EPSILON && j_end + 1 < nVector.y())j_end++;

    float z_pieces = (box_MaxP.z() - minP.z()) / deltaZ;
    if(abs(z_pieces - (int)(z_pieces+EPSILON))< EPSILON && k_end + 1 < nVector.z())k_end++;
    
    
    // Loop all the voxel in the grid
    for(int i = 0; i < nVector.x(); i++)
        for(int j = 0; j < nVector.y(); j++)
            for(int k = 0; k < nVector.z(); k++){
                int index = nVector.y() * nVector.z() * i + nVector.z() * j + k;
                if(m != NULL){  // With Transform, we just add all cells in the range of i,j,k.
                    if(i >= i_begin && i <= i_end && j >= j_begin && j <= j_end && k >= k_begin && k <= k_end){
                        g->addCellObject(index,new Transform(*m,this));
                    }
                } else {  // Without Transform, we need to perform the grid more like a sphere
                    Vec3f voxelCenter = Vec3f(minP.x() + (i + 0.5f) * deltaX,
                                          minP.y() + (j + 0.5f) * deltaY,
                                          minP.z() + (k + 0.5f) * deltaZ);
                    if((voxelCenter - center).Length() < radius + deltaL){    // Every voxel touch the sphere should be labeled opaque.
                        g->addCellObject(index,this);
                    }
                }
            }
    box = boxSaver; // Recover the origin box.
}


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
                    glNormal3f(-p0.x(),-p0.y(),-p0.z());
                    p0 += center;
                    glVertex3f(p0.x(),p0.y(),p0.z());

                    glNormal3f(-p1.x(),-p1.y(),-p1.z());
                    p1 += center;
                    glVertex3f(p1.x(),p1.y(),p1.z());

                    glNormal3f(-p2.x(),-p2.y(),-p2.z());
                    p2 += center;
                    glVertex3f(p2.x(),p2.y(),p2.z());

                    glNormal3f(-p3.x(),-p3.y(),-p3.z());
                    p3 += center;
                    glVertex3f(p3.x(),p3.y(),p3.z());
                } else {    //just set normal for this little plane
                    p0 += center;p1 += center;p2 += center;p3 += center;
                    glNormal3f(-quadNormal.x(),-quadNormal.y(),-quadNormal.z());
                    glVertex3f(p0.x(),p0.y(),p0.z());
                    glVertex3f(p1.x(),p1.y(),p1.z());
                    glVertex3f(p2.x(),p2.y(),p2.z());
                    glVertex3f(p3.x(),p3.y(),p3.z());
                } 
                
            }
    glEnd();
    cout<<"Here we paint a sphere"<<endl;
}
#endif