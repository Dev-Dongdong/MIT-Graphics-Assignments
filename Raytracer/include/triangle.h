/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-25 19:55:16
 * @LastEditTime: 2021-03-02 17:42:27
 * @LastEditors: LvBujing
 */
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "transform.h"

class Triangle : public Object3D{

private:
    Vec3f v0,v1,v2;

public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *mat) : v0(a),v1(b),v2(c),Object3D(mat){
         Vec3f minP = Vec3f(min(a.x(),min(b.x(),c.x())), min(a.y(),min(b.y(),c.y())), min(a.z(),min(b.z(),c.z())));
         Vec3f maxP = Vec3f(max(a.x(),max(b.x(),c.x())), max(a.y(),max(b.y(),c.y())), max(a.z(),max(b.z(),c.z())));
         box = new BoundingBox(minP,maxP);
         //cout<<"here triangle's boundingbox is: ";box->Print();
    }
    ~Triangle(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    void insertIntoGrid(Grid *g, Matrix *m);
    void paint(void);
};

bool Triangle::intersect(const Ray &r, Hit &h, float tmin){
    RayTracingStats::IncrementNumIntersections();
    Vec3f O = r.getOrigin();
    Vec3f D = r.getDirection();

    Vec3f E1 = v1 - v0;
    Vec3f E2 = v2 - v0;
    Vec3f S = O - v0;
    Vec3f S1,S2;
    Vec3f::Cross3(S1,D,E2);
    Vec3f::Cross3(S2,S,E1);

    float denom = 1.0f / S1.Dot3(E1);
    float t = S2.Dot3(E2) * denom;
    float b = S1.Dot3(S) * denom;
    float c = S2.Dot3(D) * denom;
    float a = 1 - b - c;
    
    if(a < 0 || a > 1 || abs(a) < 0.000001)return false;   // Little trick
    if(b < 0 || b > 1 )return false;
    if(c < 0 || c > 1 )return false;

    if(t < tmin || t > h.getT() || abs(t - tmin) < EPSILON || abs(t - h.getT()) < EPSILON)return false;

    Vec3f normal;
    Vec3f::Cross3(normal,E1,E2);
    normal.Normalize();
    h.set(t,m,normal,r);
    //cout<<"hit!"<<endl;
    return true;
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m){
    BoundingBox* boxSaver = box;
    Vec3f vertexSaver0 = v0,vertexSaver1 = v1,vertexSaver2 = v2;
    if(m != NULL) {
        //  Transform the 3 vertexes using matrix m, and then get the tighter boundingbox.
        m->Transform(v0);
        m->Transform(v1);
        m->Transform(v2);
        Vec3f newminP = Vec3f(min(v0.x(),min(v1.x(),v2.x())), min(v0.y(),min(v1.y(),v2.y())), min(v0.z(),min(v1.z(),v2.z())));
        Vec3f newmaxP = Vec3f(max(v0.x(),max(v1.x(),v2.x())), max(v0.y(),max(v1.y(),v2.y())), max(v0.z(),max(v1.z(),v2.z())));
        box = new BoundingBox(newminP,newmaxP);
        //cout<<"After transform triangle's box is: ";
        //box->Print();
    }
    int i_begin,i_end,j_begin,j_end,k_begin,k_end;
    Vec3f box_MinP = box->getMin();
    Vec3f box_MaxP = box->getMax();
    Vec3f nVector = g->getNVector();
    Vec3f minP = g->getBoundingBox()->getMin();
    Vec3f maxP = g->getBoundingBox()->getMax();
    float deltaX = (maxP.x() - minP.x()) / nVector.x();
    float deltaY = (maxP.y() - minP.y()) / nVector.y();
    float deltaZ = (maxP.z() - minP.z()) / nVector.z();

    /*
    i_begin = (box_MinP.x() - minP.x()) / deltaX + EPSILON; j_begin = (box_MinP.y() - minP.y()) / deltaY + EPSILON; k_begin = (box_MinP.z() - minP.z()) / deltaZ + EPSILON;
    i_end = (box_MaxP.x() - minP.x()) / deltaX - EPSILON; j_end = (box_MaxP.y() - minP.y()) / deltaY - EPSILON; k_end = (box_MaxP.z() - minP.z()) / deltaZ - EPSILON;
    // Deal with a lot of special cases in inserting.(Most of the case the boudingbox's plane is in grid boudingbox's plane.)
    if(abs(nVector.x() - i_begin) < EPSILON)i_begin--;
    if(abs(nVector.y() - j_begin) < EPSILON)j_begin--;
    if(abs(nVector.z() - k_begin) < EPSILON)k_begin--;
    
    // The following if is neccessary for triangles which are parralel with the box plane.
    float x_pieces = (box_MaxP.x() - minP.x()) / deltaX;
    if(abs(x_pieces - (int)(x_pieces+EPSILON))< EPSILON && i_end + 1 < nVector.x() && i_end != i_begin)i_end++;// Here i_end == i_begin is for trangles which is parralel with the box's x plane.
    
    float y_pieces = (box_MaxP.y() - minP.y()) / deltaY;
    if(abs(y_pieces - (int)(y_pieces+EPSILON))< EPSILON && j_end + 1 < nVector.y() && j_end != j_begin)j_end++;

    float z_pieces = (box_MaxP.z() - minP.z()) / deltaZ;
    if(abs(z_pieces - (int)(z_pieces+EPSILON))< EPSILON && k_end + 1 < nVector.z() && k_end != k_begin)k_end++;
    */
    i_begin = (box_MinP.x() - minP.x()) / deltaX - EPSILON; j_begin = (box_MinP.y() - minP.y()) / deltaY - EPSILON; k_begin = (box_MinP.z() - minP.z()) / deltaZ - EPSILON;
    i_end = (box_MaxP.x() - minP.x()) / deltaX + EPSILON; j_end = (box_MaxP.y() - minP.y()) / deltaY + EPSILON; k_end = (box_MaxP.z() - minP.z()) / deltaZ + EPSILON;
    
    if(abs(i_end - nVector.x()) < EPSILON)i_end--;
    if(abs(j_end - nVector.y()) < EPSILON)j_end--;
    if(abs(k_end - nVector.z()) < EPSILON)k_end--;
    if(i_end < i_begin)i_end = i_begin;
    if(j_end < j_begin)j_end = j_begin;
    if(k_end < k_begin)k_end = k_begin;
    //cout<<"#Begin "<<i_begin<<" "<<j_begin<<" "<<k_begin<<endl;
    //cout<<"#End "<<i_end<<" "<<j_end<<" "<<k_end<<endl;
    for(int i = i_begin; i <= i_end; i++)
        for(int j = j_begin; j <= j_end; j++)
            for(int k = k_begin; k <= k_end; k++){
                int index = nVector.y() * nVector.z() * i + nVector.z() * j + k;
                //cout<<"here add cell in index: "<<index<<endl;
                if(m != NULL)g->addCellObject(index,new Transform(*m,this));
                else g->addCellObject(index,this);
            }
    //  Recovery
    box = boxSaver;
    v0 = vertexSaver0;
    v1 = vertexSaver1;
    v2 = vertexSaver2;
}

void Triangle::paint(void){
    m->glSetMaterial();
    
    Vec3f E1 = v1 - v0;
    Vec3f E2 = v2 - v0;
    Vec3f normal;
    Vec3f::Cross3(normal,E1,E2);
    normal.Normalize();
    
    glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(),normal.y(),normal.z());
        glVertex3f(v0.x(),v0.y(),v0.z());
        glVertex3f(v1.x(),v1.y(),v1.z());
        glVertex3f(v2.x(),v2.y(),v2.z());
    glEnd();
}
#endif