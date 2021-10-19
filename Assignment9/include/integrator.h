/*
 * @Description: A set of Integrator
 * @Author: LvBujing
 * @Date: 2021-03-11 20:07:51
 * @LastEditTime: 2021-03-13 13:55:18
 * @LastEditors: LvBujing
 */
#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "particle.h"
#include "forceField.h"

//  The base class of Integrator
class Integrator {
    
private:
 
public:

    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    virtual Vec3f getColor() = 0;
};

//  A brand new class of EulerIntegrator 
class EulerIntegrator : public Integrator{
    
private:
    /* data */
public:
    EulerIntegrator () { }
    ~EulerIntegrator () { }

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        //  Obtain the infor about particle
        Vec3f position = particle->getPosition();
        float mass = particle->getMass();
        Vec3f velocity = particle->getVelocity();
        Vec3f a = forcefield->getAcceleration(position, mass, t);
        
        //  Update velocity, acceleration, lifespan
        position += velocity * dt;
        velocity += a * dt;
        particle->setPosition(position);
        particle->setVelocity(velocity);
        particle->increaseAge(dt);
    }

    Vec3f getColor() { return Vec3f(1,0,0); }   //  Use Red to represent Euler integrator
};

//  A brand new class of MidpointIntegrator 
class MidpointIntegrator : public Integrator{
    
private:
    /* data */
public:
    MidpointIntegrator() { }
    ~MidpointIntegrator() { }

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        //  Obtain the infor about particle
        Vec3f position = particle->getPosition();
        float mass = particle->getMass();
        Vec3f velocity = particle->getVelocity();
        Vec3f a = forcefield->getAcceleration(position, mass, t);

        //  Calculate the midpoint state
        Vec3f position_m = position + velocity * 0.5f * dt;
        Vec3f velocity_m = velocity + a * 0.5f * dt;
        a = forcefield->getAcceleration(position_m, mass, t+0.5*dt);
        
        //  Update velocity, acceleration, lifespan
        position += velocity_m * dt;
        velocity += a * dt;
        particle->setPosition(position);
        particle->setVelocity(velocity);
        particle->increaseAge(dt);
    }

    Vec3f getColor() { return Vec3f(0,1,0); }   //  Use Green to represent Midpoint integrator
};


//  TODO: A brand new class of RungeKuttaIntegrator 
class RungeKuttaIntegrator : public Integrator{
    
private:
    /* data */
public:
    RungeKuttaIntegrator() { }
    ~RungeKuttaIntegrator() { }

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        //  Obtain the infor about particle
        Vec3f position = particle->getPosition();
        float mass = particle->getMass();
        Vec3f velocity = particle->getVelocity();

        //  Calculate k1,k2,k3,k4 for RungeKutta 4 version (order-four)
        Vec3f k1 = forcefield->getAcceleration(position, mass, t);

        Vec3f v1 = velocity + k1 * dt * 0.5f;
        Vec3f p1 = position + v1 * dt * 0.5f; 
        Vec3f k2 = forcefield->getAcceleration(p1, mass, t + 0.5f*dt);

        Vec3f v2 = velocity + k2 * dt * 0.5f;
        Vec3f p2 = position + v2 * dt * 0.5f;
        Vec3f k3 = forcefield->getAcceleration(p2, mass, t + 0.5f * dt);

        Vec3f v3 = velocity + k3 * dt;
        Vec3f p3 = position + v3 * dt;
        Vec3f k4 = forcefield->getAcceleration(p3, mass, t + dt);

        //  Update velocity, acceleration, lifespan
        Vec3f new_velocity = velocity + ((float)1 / 6) * dt * (k1 + 2*k2 + 2*k3 + k4);
        Vec3f new_position = position + ((float)1 / 6) * dt * (velocity + 2*v1 + 2*v2 + v3);
        particle->setPosition(new_position);
        particle->setVelocity(new_velocity);
        particle->increaseAge(dt);
    }

    Vec3f getColor() { return Vec3f(0,0,1); }   //  Use Blue to represent RungeKuttaIntegrator
};

#endif