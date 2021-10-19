/*
 * @Description: Trace the ray in a scene.
 * @Author: LvBujing
 * @Date: 2021-01-05 10:42:00
 * @LastEditTime: 2021-03-01 18:38:11
 * @LastEditors: LvBujing
 */
#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_
#include "scene_parser.h"
#include "rayTree.h"
#include "grid.h"
#include "matrix.h"
#include "raytracing_stats.h"
#define EPSILON 0.00001f
#define SHADOWEPSILON 0.0001f

bool ifGrid = false, ifGridVisual = false;
int nx,ny,nz;
class RayTracer{
    
private:
    SceneParser *p;
    Grid *g;
    int max_bounces;
    float cutoff_weight;
    bool shadows,ifShadeBack;
public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    RayTracer(SceneParser *p, int max_bounces, float cutoff_weight, 
            bool shadows, bool ifShadeBack):p(p),max_bounces(max_bounces),
            cutoff_weight(cutoff_weight),shadows(shadows),ifShadeBack(ifShadeBack){
                
                if (!ifGrid) g = NULL;
                else{
                    g = new Grid(p->getGroup()->getBoundingBox(),nx,ny,nz);
                    g->setMat(p->getMaterial(0));
                    p->getGroup()->insertIntoGrid(g,NULL);  // Insert all the object into grid
                } 
                
                /*
                //g->Print();
                //g->paint();
                Vec3f DIR = Vec3f(0.5,0.5,4);
                DIR.Normalize();
                Vec3f ORIGIN = Vec3f(0,0,-2);
                Ray r(ORIGIN,DIR);
                Hit h;
                g->intersect(r,h,EPSILON);
                cout<<h<<endl;
                */
            }
    ~RayTracer(){if(p!=NULL)delete p;}

    // ========
    // FUNCTION
    
    // Trace a ray in the scene and return colors
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &hit);
    bool rayCast(const Ray &r, Hit &h, float tmin);
    bool fastRayCast(const Ray &r, Hit &h, float tmin); // Found intersection with grid acceleration.
    Grid* getGrid(){ return g;}
};

// calculate the outcoming dir (Remember that imcoming dir dot normal < 0)
Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming){
    float K = normal.Dot3(incoming);
    Vec3f outcoming = (incoming - K * normal) - (K * normal);
    outcoming.Normalize();
    return outcoming;
}
bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, 
      float index_i, float index_t, Vec3f &transmitted){
    float ior = index_i / index_t;
    float cosTheta1 = abs(incoming.Dot3(normal));
    if(1 - cosTheta1 * cosTheta1 > 1 / ior / ior)return false;  // Total internal reflection
    float cosTheta2 = sqrt(1 - (1 - cosTheta1 * cosTheta1) * ior * ior);
    
    if(ior < 1){
        transmitted = incoming - (cosTheta2 / ior - cosTheta1)  * normal;
    }else{
        transmitted = incoming - normal * (cosTheta1 - cosTheta2 / ior);
    }
    transmitted.Normalize();
    return true;
}
Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &hit){
    RayTracingStats::IncrementNumNonShadowRays();   // Everytime call traceRay there must be a non-shadow ray
    if(bounces > max_bounces || weight < cutoff_weight){  // the bounce time exceed the max_bounce
        return Vec3f(0,0,0);
    }

    bool ifIntersect = false;
    //RayTree::SetMainSegment(ray,0,hit.getT());
    
   
    if(ifGridVisual){ // Grid Occupancy is open, DO NOT render the scene
        ifIntersect = g->intersect(ray,hit,tmin);
    } else {
        if(ifGrid) {
            ifIntersect = fastRayCast(ray,hit,tmin); // Grid exists then use it for acceleration
        }
        else ifIntersect = rayCast(ray,hit,tmin);
    }
    

    if(ifIntersect){    //if this ray hit a object, the intersection information will be stored at 'hit'
        // frequently used variables
        if(bounces == 0)RayTree::SetMainSegment(ray,0,hit.getT());
        Vec3f color(0,0,0);
        Vec3f normal = hit.getNormal();
        Vec3f position = hit.getIntersectionPoint();
        Material *m = hit.getMaterial();
        
        // deal with reflective material.
        if(m->getReflectiveColor(position).Length() > EPSILON){
            Vec3f reflectedDir = mirrorDirection(normal,ray.getDirection());
            Ray reflectedRay(position,reflectedDir);
            Hit reflectedInter;
            color += m->getReflectiveColor(position) * traceRay(reflectedRay,EPSILON,bounces+1,weight * m->getReflectiveColor(position).Length(),indexOfRefraction,reflectedInter);
            
            RayTree::AddReflectedSegment(reflectedRay,0,reflectedInter.getT());
        }

        // deal with transparent material
        if(m->getTransparentColor(position).Length() > EPSILON){
            Vec3f transmittedDir;
            bool canTransmitted, isInside = normal.Dot3(ray.getDirection()) > 0;
            if(!isInside)canTransmitted = transmittedDirection(normal,ray.getDirection(),indexOfRefraction,m->getIndexOfRefraction(position),transmittedDir);
            else canTransmitted = transmittedDirection(normal,ray.getDirection(),indexOfRefraction,1,transmittedDir);
            if(canTransmitted){
                Ray transmittedRay(position,transmittedDir);                
                Hit transmittedInter;
                float nowFractionInd = (isInside) ? 1 : m->getIndexOfRefraction(position);
                color += m->getTransparentColor(position) * traceRay(transmittedRay,EPSILON,bounces+1,weight * m->getTransparentColor(position).Length(),nowFractionInd,transmittedInter);
                RayTree::AddTransmittedSegment(transmittedRay,0,transmittedInter.getT());
            }
        }
        
        // ===============================
        // obtain the color for this pixel
        Vec3f lightSumColor = Vec3f(0,0,0);
        float NdotL;
        
        //if(ifShadeBack && normal.Dot3(ray.getDirection()) > 0)normal.Negate();  //render the back surface as well
        if(ifShadeBack && normal.Dot3(ray.getDirection()) > 0){ //render the back surface as well(This method will be better than above one)
            normal.Negate();
            hit.set(hit.getT(),hit.getMaterial(),normal,ray);
        }

        // Go through all the lights in scene
        for(int k = 0; k < p->getNumLights(); k++){
            Light* L = p->getLight(k);
            Vec3f lightDir,lightColor;
            float distanceToLight;
            L->getIllumination(position,lightDir,lightColor,distanceToLight); // Obtain the information about light source.
            
            // Deal with shadows 
            if(shadows){
                //Vec3f transparentAttenuation(0.05f,0.05f,0.05f);
                Hit shadowCover;
                Ray lightRay(position,lightDir);
                RayTracingStats::IncrementNumShadowRays();
                bool isBlocked;
                
                if(ifGrid) {    // Use grid acceleration for shadow ray intersection
                    isBlocked = fastRayCast(lightRay,shadowCover,SHADOWEPSILON); // Grid exists then use it for acceleration
                } else isBlocked = rayCast(lightRay,shadowCover,SHADOWEPSILON); // Here EPSILON is too small for correct self-shadow check
                RayTree::AddShadowSegment(lightRay,0,shadowCover.getT());
                if(isBlocked && shadowCover.getT() < distanceToLight){  // There is something bewteen shaderpoint and light
                    if(shadowCover.getMaterial()->getTransparentColor(position).Length() > EPSILON){    // If "something" is transparent object.
                        /*
                        // TODO: complete the semi-transparent shadows(now is naive method which is not friendly to diamond.
                        isBlocked = false;
                        Hit anotherSide;
                        Ray newRay(shadowCover.getIntersectionPoint(),lightDir);
                        float totalDis = shadowCover.getT();
                        while(p->getGroup()->intersect(newRay,anotherSide,EPSILON) && totalDis + anotherSide.getT() < distanceToLight){    // a lot of transparent objects.
                            totalDis += anotherSide.getT();
                            if(anotherSide.getMaterial()->getTransparentColor().Length() < EPSILON){    // the shadow ray hit something "opaque", here is also shadow.
                                isBlocked=true;
                                break;
                            }
                            // TODO: Find a weight to decide lightColor.
                            if(anotherSide.getNormal().Dot3(lightDir) > 0)lightColor = lightColor * (anotherSide.getMaterial()->getTransparentColor() - transparentAttenuation * anotherSide.getT());
                            newRay = Ray(anotherSide.getIntersectionPoint(),lightDir);
                            anotherSide = Hit();
                        }
                        if(isBlocked)continue;
                        */
                    }
                    else continue;  //not the transparent object, here is shadow
                    continue;   // use semi-transparent method please comment on this line.
                }
            }
            
            //NdotL = normal.Dot3(lightDir);
            //if(NdotL < 0)NdotL = 0; //Clamp. Not deal with the back surface
            
            
            //specular = m->Shade(ray,hit,lightDir,lightColor,distanceToLight);
            //lightSumColor += NdotL * lightColor * diffuse + specular;
            lightSumColor += m->Shade(ray,hit,lightDir,lightColor,distanceToLight);
        }
        color += p->getAmbientLight() * m->getDiffuseColor(position) + lightSumColor;   // Add the ambient    
        return color;
    } else return p->getBackgroundColor();
}

//  Ordinary intersect func. Check all objects one by one
bool RayTracer::rayCast(const Ray &r, Hit &h, float tmin) {
    return p->getGroup()->intersect(r,h,tmin);
}

//  Faster intersect func. Only check objects along the ray (Grid Acceleration)
bool RayTracer::fastRayCast(const Ray &r, Hit &h, float tmin) {
    return g->fastIntersect(r,h,tmin);
}


#endif