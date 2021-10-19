/*
 * @Description: A set classes of generator
 * @Author: LvBujing
 * @Date: 2021-03-11 20:32:13
 * @LastEditTime: 2021-03-13 14:41:15
 * @LastEditors: LvBujing
 */
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "particle.h"
#include "random.h"
#include "GL/glut.h"
#include "GL/gl.h"
#define MY_PI 3.14159265
//  The base class of generator
class Generator {

protected:
    Random* randomGenerator;
    Vec3f color, dead_color;
    float color_randomness;
    float lifespan, lifespan_randomness, desired_num_particles;
    float mass, mass_randomness;
public:
    Generator () { randomGenerator = new Random(); }
    ~Generator() { delete randomGenerator; }

    // initialization
    void SetColors(Vec3f _color, Vec3f _dead_color, float _color_randomness) { 
        color = _color; dead_color = _dead_color; color_randomness = _color_randomness; } 
    void SetLifespan(float _lifespan, float _lifespan_randomness, int _desired_num_particles) {
        lifespan = _lifespan; lifespan_randomness = _lifespan_randomness; desired_num_particles = _desired_num_particles;}
    void SetMass(float _mass, float _mass_randomness) { mass = _mass; mass_randomness = _mass_randomness; }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const { 
        int num = (dt * desired_num_particles / lifespan);
        return (num == 0) ? 1 : num; 
    }
    virtual Particle* Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const = 0;
    void Restart() { delete randomGenerator; randomGenerator = new Random(); }  //  Reset the random generator
};

//  A brand new class of HoseGenerator 
class HoseGenerator : public Generator {

private:
    Vec3f position, velocity;
    float position_randomness, velocity_randomness;
public:
    HoseGenerator(Vec3f _position, float _position_randomness, Vec3f _velocity, float _velocity_randomness) :
        position(_position), position_randomness(_position_randomness), velocity(_velocity), velocity_randomness(_velocity_randomness) { }
    ~HoseGenerator() { }

    void Paint() const {
        
    }

    Particle* Generate(float current_time, int i) {
        Vec3f p_position = position + randomGenerator->randomVector() * position_randomness;
        Vec3f p_velocity = velocity + randomGenerator->randomVector() * velocity_randomness;
        Vec3f p_color = color + randomGenerator->randomVector() * color_randomness;
        Vec3f p_dead_color = dead_color;
        float p_mass = mass + randomGenerator->next() * mass_randomness;
        float p_lifespan = lifespan + randomGenerator->next() * lifespan_randomness;
        Particle* p = new Particle(p_position, p_velocity, p_color, p_dead_color, p_mass, p_lifespan);
        return p;
    }
};

//  A brand new class of RingGenerator 
class RingGenerator : public Generator {

private:
    Vec3f velocity;
    float position_randomness, velocity_randomness;
public:
    RingGenerator(float _position_randomness, Vec3f _velocity, float _velocity_randomness) :
        position_randomness(_position_randomness), velocity(_velocity), velocity_randomness(_velocity_randomness) { }
    ~RingGenerator() { }

    void Paint() const {
        glColor3f(0.28f, 0.28f, 0.28f);
        glBegin(GL_QUADS);
            glVertex3f(-4,0,-4);
            glVertex3f(4,0,-4);
            glVertex3f(4,0,4);
            glVertex3f(-4,0,4);
        glEnd();
    }

    Particle* Generate(float current_time, int i) {
        float angle = 2 * MY_PI * randomGenerator->next();
        Vec3f p_position = Vec3f(current_time * cos(angle), 0, current_time * sin(angle));
        Vec3f p_velocity = velocity + randomGenerator->randomVector() * velocity_randomness;
        Vec3f p_color = color + randomGenerator->randomVector() * color_randomness;
        Vec3f p_dead_color = dead_color;
        float p_mass = mass + randomGenerator->next() * mass_randomness;
        float p_lifespan = lifespan + randomGenerator->next() * lifespan_randomness;
        Particle* p = new Particle(p_position, p_velocity, p_color, p_dead_color, p_mass, p_lifespan);
        return p;
    }

    // on each timestep, create some particles
    int numNewParticles(float current_time, float dt) const { 
        int num = (dt * desired_num_particles / lifespan);
        return (num == 0) ? current_time : num * current_time; 
    }
};




#endif