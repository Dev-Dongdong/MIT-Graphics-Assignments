/*
 * @Description: The base class of all 3D object 
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2021-02-09 14:31:43
 * @LastEditors: LvBujing
 */
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h" 
#include "vectors.h"
#include "hit.h"
#include "boundingbox.h"
#include "matrix.h"
#include "raytracing_stats.h"
#include <GL/gl.h>
#define EPSILON 0.00001f
class Grid;

class Object3D {
    
protected:
    Material* m;
    BoundingBox *box;
public: 
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Object3D(){m = NULL; box = NULL;}
    Object3D(Material *mat){m = mat; box = NULL; }
    virtual ~Object3D(){
        if(m != NULL)delete m; }

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    //  The naive way to handle Transform.(Any rotation will make the boundingbox become bigger)
    //  #Attention#: This func will pernamently change the box of this object3D, remember to store before call this func and recover later.
    virtual void insertIntoGrid(Grid *g, Matrix *m){
        Vec3f minPBox ,maxPBox;
        calculateBox(minPBox,maxPBox,box,m);
        box = new BoundingBox(minPBox,maxPBox);
    }

    BoundingBox* getBoundingBox(){ return box; }

    virtual void paint(void) = 0;

    // Using the transMatrix to transform the 8 corner of boundingbox b, and store the result boundingbox's minP and maxP.
    void calculateBox(Vec3f &minP, Vec3f &maxP, BoundingBox *b, const Matrix* transMatrix){
        Vec3f corner[8];
        corner[0] = b->getMin();
        corner[7] = b->getMax();
        float disx,disy,disz;
        disx = corner[7].x() - corner[0].x();
        disy = corner[7].y() - corner[0].y();
        disz = corner[7].z() - corner[0].z();
        
        corner[1] = Vec3f(corner[0].x() + disx, corner[0].y(),        corner[0].z());
        corner[2] = Vec3f(corner[0].x(),        corner[0].y() + disy, corner[0].z());
        corner[3] = Vec3f(corner[0].x() + disx, corner[0].y() + disy, corner[0].z());

        corner[4] = Vec3f(corner[7].x() - disx, corner[7].y() - disy, corner[7].z());
        corner[5] = Vec3f(corner[7].x() - disx, corner[7].y(),        corner[7].z());
        corner[6] = Vec3f(corner[7].x(),        corner[7].y() - disy, corner[7].z());

        // Transform all the 8 corners.
        for(int i = 0; i < 8; i++){
            transMatrix->Transform(corner[i]);
        }
        maxP = minP = corner[0];
        // Find the new boundingbox.
        for(int i = 1; i < 8; i++){
            maxP.Set(   max(    maxP.x(),   corner[i].x()   ),
                        max(    maxP.y(),   corner[i].y()   ),
                        max(    maxP.z(),   corner[i].z()   ) );
            minP.Set(   min(    minP.x(),   corner[i].x()   ),
                        min(    minP.y(),   corner[i].y()   ),
                        min(    minP.z(),   corner[i].z()   ) );
        }
    }
};




#endif