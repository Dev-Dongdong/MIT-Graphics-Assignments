
#ifndef _IFS_H_
#define _IFS_H_
/*
 * @Description: 
 * @Author: LvBujing
 * @Date: 2020-12-23 13:57:00
 * @LastEditTime: 2020-12-23 16:16:43
 * @LastEditors: LvBujing
 */
#include "matrix.h"
#include "image.h"
#include "vectors.h"
#include <vector>
class IFS{
private:
    // number of transforms
    int n;
    
    // matrices of transforms
    Matrix* matrices;

    // probabilities of transforms
    float* probabilities;  //add all probabilities together will get 1 


public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================

    IFS(int num){
        n = num;
        matrices = new Matrix[num];
        probabilities = new float[num];
    }
    ~IFS(){
        delete [] matrices;
        delete [] probabilities;   
    }

    // =========
    // ACCESSORS
    // =========
    Matrix get_probability_based_matrix();
    float get_random_nums();

    // =========
    // MODOFIERS
    // =========

    // add single matrics and corresponding probability.
    void addMatricesAndProbability(const int &i,const Matrix &m1,const float &p);

    // ========
    // FUNCTION
    // ========
    // render a image use IFS
    void render(Image* image,const int &n_points,const int &n_iteration);
};


#endif
