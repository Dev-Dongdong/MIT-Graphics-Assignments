/*
 * @Description: A class of CheckerBoard
 * derive from matrial
 * @Author: LvBujing
 * @Date: 2021-02-24 14:54:12
 * @LastEditTime: 2021-03-01 10:44:10
 * @LastEditors: LvBujing
 */

#include "material.h"
#include "matrix.h"
#include "phongMaterial.h"

class CheckerBoard : public Material {

private:
    Material *mat1,*mat2;
    Matrix *m;

    // Distribute material depending on pos (Here pos is after transforming.)
    bool matDistribute(Vec3f &pos) const {
        bool xOdd,yOdd,zOdd;
        xOdd = ((int)floor(pos.x()) % 2 == 0) ? false : true;
        yOdd = ((int)floor(pos.y()) % 2 == 0) ? false : true;
        zOdd = ((int)floor(pos.z()) % 2 == 0) ? false : true;
        //cout<<xOdd<<" "<<yOdd<<" "<<zOdd<<endl;
        if(xOdd){
            if(yOdd) return !zOdd;
            else return zOdd;
        } else {
            if (yOdd) return zOdd;
            else return !zOdd; 
        }
    }
public:
    CheckerBoard(Matrix *_m, Material *_mat1, Material *_mat2) : mat1(_mat1),mat2(_mat2),m(_m) { }
    ~CheckerBoard() { }//if (mat1 != NULL) delete mat1; if (mat2 != NULL) delete mat2; if (m != NULL) delete m; }
    void glSetMaterial(void) const { mat1->glSetMaterial(); }
    Vec3f getDiffuseColor(const Vec3f &pos) const { 
        Vec3f tempPos = pos;
        m->Transform(tempPos);
        if (matDistribute(tempPos)) return mat1->getDiffuseColor(pos);
        else return mat2->getDiffuseColor(pos);
    }

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor, const float &distance) const {
        Vec3f pos = hit.getIntersectionPoint();
        m->Transform(pos);
        if (matDistribute(pos)) return mat1->Shade(ray,hit,dirToLight,lightColor,distance);
        else return mat2->Shade(ray,hit,dirToLight,lightColor,distance);
    }
};

