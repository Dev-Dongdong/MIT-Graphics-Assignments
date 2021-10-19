/*
 * @Description: A set classes of forcefield, it generate a force based on position
 * @Author: LvBujing
 * @Date: 2021-03-11 19:42:33
 * @LastEditTime: 2021-10-07 12:54:41
 * @LastEditors: LvBujing
 */
#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "perlin_noise.h"
#include "vectors.h"
//  The Base class of ForceField
class ForceField {
    
private:
    /* data */
public:
    ForceField(/* args */) { }
    ~ForceField() { }

    //  Key Func
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

//  A brand new GravityForceField
class GravityForceField : public ForceField {

private:
    Vec3f gravity;
public:
    GravityForceField(Vec3f _gravity) : gravity(_gravity) { }
    ~GravityForceField() { }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return gravity; }
};

//  A brand new ConstantForceField
class ConstantForceField : public ForceField {

private:
    Vec3f force;
public:
    ConstantForceField(Vec3f _force) : force(_force) { } 
    ~ConstantForceField() { }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { return force * (1 / mass); }
};

//  A brand new RadialForceField
class RadialForceField : public ForceField {

private:
    float magnitude;
public:
    RadialForceField(float _magnitude) : magnitude(_magnitude) { }
    ~RadialForceField() { }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { Vec3f grab = -1 * position; return magnitude * grab; }
};

//  A brand new VerticalForceField
class VerticalForceField : public ForceField {

private:
    float magnitude;
public:
    VerticalForceField(float _magnitude) : magnitude(_magnitude) { }
    ~VerticalForceField() { }

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { Vec3f grab = Vec3f(0, -position.y(), 0); return magnitude * grab; }
};

//  A brand new WindForceField
class WindForceField : public ForceField {

private:
    float magnitude;
public:
    WindForceField(float _magnitude) : magnitude(_magnitude) { }
    ~WindForceField() { }

    //  Make it like a wind
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const { 
        float swing = PerlinNoise::noise(position.x(), position.y(), t);
        Vec3f disturb = Vec3f(-swing * magnitude,0,0);
        return disturb;
    }
};

#endif