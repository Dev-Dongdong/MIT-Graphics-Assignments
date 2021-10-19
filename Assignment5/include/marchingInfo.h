/*
 * @Description: Store the infomation for current ray and current gird cell.
 * @Author: LvBujing
 * @Date: 2021-01-19 20:56:58
 * @LastEditTime: 2021-01-31 14:11:46
 * @LastEditors: LvBujing
 */
#ifndef _MARCHINGINFO_H_
#define _MARCHINGINFO_H_

#include "vectors.h"
class MarchingInfo{
    
private:
    float tmin; //  current tmin
    int i,j,k;  //  current grid cell index
    Vec3f t_next,d_t,sign,normal;  // t_next: next values of intersection along each axis
                                   // d_t: marching increments along each axis
                                   // sign: sign of the direction vector components
                                   // normal: the normal of face which was crossed
    float nowT;
    
public:
    MarchingInfo(){tmin = INFINITY; i = j = k = -1; t_next = d_t = sign = normal = Vec3f(0,0,0); nowT = 0; }
    ~MarchingInfo(){}
    void get(float &_tmin, int &_i, int &_j, int &_k, Vec3f &_t_next, Vec3f &_d_t, Vec3f &_sign, Vec3f &_normal){
        _tmin = tmin; _i = i; _j = j; _k = k; _t_next = t_next; _d_t = d_t; _sign = sign; _normal = normal;
    }
    Vec3f getGridIndex(){ return Vec3f(i,j,k); }
    int getGridIndexI(){ return i; }
    int getGridIndexJ(){ return j; }
    int getGridIndexK(){ return k;}
    float getNowT(){ return nowT; }
    Vec3f getNormal(){ return normal; }
    void set(float &_tmin, int &_i, int &_j, int &_k, Vec3f &_t_next, Vec3f &_d_t, Vec3f &_sign, Vec3f &_normal, float _nowT){
        tmin = _tmin; i = _i; j = _j; k = _k; t_next = _t_next; d_t = _d_t; sign = _sign; normal = _normal; nowT = _nowT;
    }
    void print(){
        cout<<"#MarchingInfo: "<<endl;
        cout<<"i: "<<i<<" j: "<<j<<" k: "<<k<<endl;
        cout<<"tmin: "<<tmin<<endl;
        cout<<"tnext: "<<t_next<<endl;
        cout<<"d_t: "<<d_t<<endl;
        cout<<"sign: "<<sign<<endl;
        cout<<"normal: "<<normal<<endl;
    }
    void nextCell(){
        float minT = min(t_next.x(),min(t_next.y(),t_next.z()));
        nowT = minT;
        if(abs(minT - t_next.x()) < EPSILON){
            t_next.Set(t_next.x() + d_t.x(), t_next.y(), t_next.z());
            i += sign.x();
            normal = Vec3f(-sign.x(),0,0);
        }
        if(abs(minT - t_next.y()) < EPSILON){
            t_next.Set(t_next.x(), t_next.y() + d_t.y(), t_next.z());
            j += sign.y();
            normal = Vec3f(0,-sign.y(),0);
        }
        if(abs(minT - t_next.z()) < EPSILON){
            t_next.Set(t_next.x(), t_next.y(), t_next.z() + d_t.z());
            k += sign.z();
            normal = Vec3f(0,0,-sign.z());
        }
    }
};

#endif