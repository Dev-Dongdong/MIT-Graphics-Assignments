/*
 * @Description: The base class of all 3D object 
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-28 16:13:05
 * @LastEditors: LvBujing
 */
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h" 
#include "vectors.h"
#include "hit.h"
#include <GL/gl.h>

class Object3D {
    
protected:
    Material* m;
public: 
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Object3D(){m = NULL; }
    Object3D(Material *mat){m = mat; }
    virtual ~Object3D(){
        if(m != NULL)delete m; }

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    virtual void paint(void) = 0;
};





#endif