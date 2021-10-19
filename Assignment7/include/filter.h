/*
 * @Description: The base class of Filter
 * @Author: LvBujing
 * @Date: 2021-03-02 15:30:43
 * @LastEditTime: 2021-03-11 16:51:25
 * @LastEditors: LvBujing
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#include "film.h"
#define EPSILON 0.00001f

class Filter {

private:
    /* data */
public:
    Filter() { }
    ~Filter() { }
    
    Vec3f getColor(int i, int j, Film *film) {
        int nums_samples = film->getNumSamples();
        int width = film->getWidth(), height = film->getHeight();
        Sample samples;
        int r = getSupportRadius();
        int i_begin = (i - r >= 0) ? (i - r) : 0, i_end = (i + r < width) ? (i + r) : (width - 1);
        int j_begin = (j - r >= 0) ? (j - r) : 0, j_end = (j + r < height) ? (j + r) : (height - 1);

        //  Calculate the weighted average of every sample
        Vec3f color = Vec3f{0,0,0};
        float sum = 0;
        for( int i2 = i_begin; i2 <= i_end; i2++ )
            for( int j2 = j_begin; j2 <= j_end; j2++ ) {
                for( int n = 0; n < nums_samples; n++ ) {
                    samples = film->getSample(i2,j2,n);
                    Vec2f insidePosition = samples.getPosition();
                    float x = i2 + insidePosition.x() - (i + 0.5f);
                    float y = j2 + insidePosition.y() - (j + 0.5f);
                    float weight = getWeight(x,y);
                    sum += weight;
                    color += weight * samples.getColor();
                }
            }
        return color * (1 / sum);    
    }
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

//  Filter-Type: Box
class BoxFilter : public Filter {

private:
    float radius;
public:
    BoxFilter(float _radius) : radius(_radius) { }
    ~BoxFilter() { }
    
    float getWeight(float x, float y) { if( abs(x) < radius && abs(y) < radius ) return 1;  //  Weight area is a box
                                        else return 0; }
    int getSupportRadius() { float check = radius - 0.5f; return (abs(check - (int)check) > EPSILON) ? (int)check+1 : (int)check; }
};

//  Filter-Type: Tent
class TentFilter : public Filter {

private:
    float radius;
public:
    TentFilter(float _radius) : radius(_radius) { }
    ~TentFilter() { }
    
    float getWeight(float x, float y) { float dis = sqrt(x*x + y*y); return (dis > radius) ? 0 : 1 - dis / radius; }    //  Weight area is a circle
    int getSupportRadius() { float check = radius - 0.5f; return (abs(check - (int)check) > EPSILON) ? (int)check+1 : (int)check; }
};

//  Filter-Type: Gaussian
class GaussianFilter : public Filter {

private:
    float sigma;
public:
    GaussianFilter(float _sigma) : sigma(_sigma) { }
    ~GaussianFilter() { }
    
    float getWeight(float x, float y) { float dis = sqrt(x*x + y* y);
                                        //if(dis > 2 * sigma)return 0;
                                        float index = - dis * dis  / (2 * sigma * sigma);
                                        return exp(index);}
    int getSupportRadius() { float check = 2 * sigma - 0.5f; return (abs(check - (int)check) > EPSILON) ? (int)check+1 : (int)check; }
};

#endif