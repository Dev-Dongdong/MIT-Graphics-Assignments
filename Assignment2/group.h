/*
 * @Description: Class of a Group, a set of Object3D
 * @Author: LvBujing
 * @Date: 2020-12-23 20:16:29
 * @LastEditTime: 2020-12-27 14:18:36
 * @LastEditors: LvBujing
 */
#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"

class Group : public Object3D{
    
private:
    // An pointer array , in which every pointer points to a Object3D
    Object3D **objects; //You can't have virtual class instance, but you do can have a pointer which points to virtual class.
    int nums;
public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================
    Group(const int &n){objects = new Object3D*[n]; nums = n; }
    ~Group(){ if(objects!=NULL) delete objects; }

    // =========
    // FUNCTIONS
    // =========
    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        bool ifHit = false;
        //cout<<"totoal nums: "<<nums<<endl;
        for(int i = 0; i < nums; i++){
            if(objects[i]->intersect(r,h,tmin)){
                ifHit = true;
            }
        }
        return ifHit;
    }
    void addObject(int index, Object3D *obj){ objects[index] = obj; }
};

#endif