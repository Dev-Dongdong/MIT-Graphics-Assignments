/*
 * @Description: Main Executable CPP
 * @Author: LvBujing
 * @Date: 2020-12-23 20:08:02
 * @LastEditTime: 2020-12-30 19:49:15
 * @LastEditors: LvBujing
 */
#include "hit.h"
#include "material.h"
#include "light.h"
#include "cstring"
#include "image.h"
#include "group.h"
#include "camera.h"
#include "glCanvas.h"
#include "GL/glut.h"
#include "scene_parser.h"

#define EPSILON 0.0001f

using namespace std;

// ===============
// Common Function
void parseCommandArg(int &argc, char** argv);

// =========
// Arguments
string input_file = "", output_file = "", depth_file = "",normal_file = "";
int width = 100;
int height = 100;
float depth_min = 0;
float depth_max = 1;
bool ifShadeBack = false;
bool ifGUI = false;
extern float phiStep, thetaStep;
extern bool isGouraud;
void render();
int main(int argc,char** argv)
{
    parseCommandArg(argc,argv);
    
    SceneParser *p = new SceneParser(input_file.c_str());

    Image *image,*depthImage,*normalImage;

    if (output_file.length() > 0) image = new Image(width,height);
    if (depth_file.length() > 0) depthImage = new Image(width,height);
    if (normal_file.length() > 0) normalImage = new Image(width,height);

    // set background color
    image->SetAllPixels(p->getBackgroundColor());
    
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            Vec2f point = {(float)i/width,(float)j/height};
            Ray ray = p->getCamera()->generateRay(point); //get ray from orthographic camera
            float tmin = p->getCamera()->getTMin(); //get tmin of this camera
            
            Hit inter;
            if(p->getGroup()->intersect(ray,inter,tmin)){   //if this ray hit a object, the intersection information will be stored at 'inter' 
                // ===============================
                // obtain the color for this pixel
                Vec3f diffuse,specular;
                assert(inter.getMaterial() != NULL);
                diffuse = inter.getMaterial()->getDiffuseColor();
                
                
                Vec3f lightSumColor = Vec3f(0,0,0);
                float NdotL;

                Vec3f normal = inter.getNormal();
                if(ifShadeBack && normal.Dot3(ray.getDirection()) > 0)normal.Negate();  //render the back surface as well
                
                // Go through all the lights in scene
                for(int k = 0; k < p->getNumLights(); k++){
                    Light* L = p->getLight(k);
                    Vec3f lightDir,lightColor;
                    float distanceToLight;
                    L->getIllumination(inter.getIntersectionPoint(),lightDir,lightColor,distanceToLight);
                    NdotL = normal.Dot3(lightDir);
                    if(NdotL < 0)NdotL = 0; //Clamp. Not deal with the back surface

                    specular = inter.getMaterial()->Shade(ray,inter,lightDir,lightColor);
                    lightSumColor += NdotL * lightColor * diffuse + specular;
                }
                Vec3f color = p->getAmbientLight() * diffuse + lightSumColor;
                image->SetPixel(i,j,color);
                
                // ==============================
                // genrate the normal color image
                if(normal_file.length() > 0){
                    Vec3f normalColor = Vec3f(abs(normal.x()),abs(normal.y()),abs(normal.z()));
                    normalImage->SetPixel(i,j,normalColor);
                }
                

                // ======================
                // tackle the depth image
                if(depth_file.length() > 0){
                    float depth = inter.getT();
                    if(depth >= depth_min && depth <= depth_max){
                        float grayscale = (depth - depth_min) / (depth_max - depth_min);
                        grayscale = 1 - grayscale;
                        Vec3f gray = Vec3f(grayscale,grayscale,grayscale);
                        depthImage->SetPixel(i,j,gray);
                    }
                }
            }
        }
    }
    
    if (output_file.length() > 0){
        output_file = "../output_images/" + output_file;
        image->SaveTGA(output_file.c_str());
    }

    if(depth_file.length() > 0){
        depth_file = "../output_images/" + depth_file;
        depthImage->SaveTGA(depth_file.c_str());
    }
    
    if(normal_file.length() > 0){
        normal_file = "../output_images/" + normal_file;
        normalImage->SaveTGA(normal_file.c_str());
    }
    
    if(ifGUI){
        glutInit(&argc,argv);
        GLCanvas *canvas = new GLCanvas();
        canvas->initialize(p,render);
    }
    
    
    delete image;
    delete depthImage;
    delete normalImage;
    
    
    delete p;
    return 0;
}

// This function parse the arguments from command.
void parseCommandArg(int &argc, char** argv){
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")){
            i++; assert(i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")){
            ifShadeBack = true;
        } else if (!strcmp(argv[i],"-gui")){
            ifGUI = true;
        } else if(!strcmp(argv[i],"-tessellation")){
            i++; assert(i < argc);
            thetaStep = atoi(argv[i]);
            i++; assert(i < argc);
            phiStep = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-gouraud")){
            isGouraud = true;
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    input_file = "../input_files/" + input_file;    //move the input_files warehouse
}

void render(){
    
    return;
}