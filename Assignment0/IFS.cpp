/*
 * @Description: Implement of IFS class
 * @Author: LvBujing
 * @Date: 2020-12-23 13:57:07
 * @LastEditTime: 2021-09-27 16:40:21
 * @LastEditors: LvBujing
 */
#include "IFS.h"
#include <iostream>

    //get random nums between [0,1]
float IFS::get_random_nums()
{   return (float)rand() / RAND_MAX;}

    //get matrix based on probability
Matrix IFS::get_probability_based_matrix()
{
    float e = get_random_nums();
    for(int i = 0; i < n; i++){
        e -= probabilities[i];
        if(e < 0)return matrices[i];
    }
    return matrices[n-1];
}
/**
 * @description: Assign value for IFS instance.
 * @param i:index
 * @param m1:matrix set in index i
 * @param p:probability set in index i
 * @return {*}
 */
void IFS::addMatricesAndProbability(const int &i,const Matrix &m1,const float &p)
{   matrices[i] = m1;probabilities[i] = p;}

/**
 * @description: render a image use IFS.
 * @param image:the image instance to save the result
 * @param n_points:total num of points to be write
 * @param n_iteration:total num of iteration
 * @return {*}
 */
void IFS::render(Image* image,const int &n_points,const int &n_iteration)
{
    for(int i = 0; i < n_points; i++){
        Vec2f point = Vec2f(get_random_nums(),get_random_nums());
        for(int j = 0; j < n_iteration; j++){
            Matrix transf = get_probability_based_matrix();
            transf.Transform(point);
        }
        int index_x = int(point.x() * image->Width());
        int index_y = int(point.y() * image->Height());
        if (index_x < 0 || index_x >= image->Width() || index_y < 0 || index_y >= image->Height()) continue;
        image->SetPixel(index_x, index_y, Vec3f(0,0,0));
    }
}