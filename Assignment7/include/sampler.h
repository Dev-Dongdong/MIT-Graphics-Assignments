/*
 * @Description: The base class of sampler
 * @Author: LvBujing
 * @Date: 2021-03-02 12:59:02
 * @LastEditTime: 2021-03-05 17:22:02
 * @LastEditors: LvBujing
 */
#ifndef _SAMPLER_H_
#define _SAMPLER_H_ 

#include "vectors.h"
#include "assert.h"
#include "ctime"

class Sampler {
    
protected:
    int num_samples;
public:
    Sampler(int _num_samples) : num_samples(_num_samples) { }
    ~Sampler() { }
    virtual Vec2f getSamplePosition(int n) = 0;
};

// Randomly sample nums points inside a pixel
class RandomSampler : public Sampler {
    
private:

public:
    RandomSampler(int _num_samples) : Sampler(_num_samples) { }
    ~RandomSampler() { }
    Vec2f getSamplePosition(int n) {    
        assert(n < num_samples);
        int N = 999;
        float x = (rand() % (N + 1)) / (float)(N + 1);
        float y = (rand() % (N + 1)) / (float)(N + 1);
        return Vec2f(x,y);
    }
};

// Uniformly sample nums points inside a pixel
class UniformSampler : public Sampler {

private:

public:
    UniformSampler(int _num_samples) : Sampler(_num_samples) { }
    ~UniformSampler() { }
    Vec2f getSamplePosition(int n) {
        assert(n < num_samples);
        int s = sqrt(num_samples);
        int row = n / s;    //  Row index of subpixel
        int col = n % s;    //  Column index of subpixel
        float delta = 1.0f / s; //  Each pixel's cell length
        float halfDelta = delta / 2.0f;
        float x = row * delta + halfDelta;
        float y = col * delta + halfDelta;
        return Vec2f(x,y);
    }
};

// Unifromly seperate the pixel into subpixels, randomly sample nums points inside each subpixel
class JitteredSampler : public Sampler {

private:

public:
    JitteredSampler(int _num_samples) : Sampler(_num_samples) { }
    ~JitteredSampler() { }
    Vec2f getSamplePosition(int n) {
        assert(n < num_samples);
        int s = sqrt(num_samples);
        int N = 999;
        int row = n / s;
        int col = n % s;
        float delta = 1.0f / s;
        float randomX = (rand() % (N + 1)) / (float)(N + 1);
        float randomY = (rand() % (N + 1)) / (float)(N + 1);
        float x = ((float)row + randomX) * delta;
        float y = ((float)col + randomY) * delta;
        return Vec2f(x,y);
    }
};

#endif