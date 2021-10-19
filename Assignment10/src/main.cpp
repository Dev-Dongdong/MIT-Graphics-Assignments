/*
 * @Description: Main Executable CPP
 * @Author: LvBujing
 * @Date: 2020-12-23 20:08:02
 * @LastEditTime: 2021-10-18 14:26:39
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
#include "raytracer.h"
#include "scene_parser.h"
#include "raytracing_stats.h"

#define EPSILON 0.00001f

using namespace std;

// ===============
// Common Function
void parseCommandArg(int &argc, char** argv);

// =========
// Arguments
string input_file = "", output_file = "", depth_file = "",normal_file = "";
int width = 100;
int height = 100;
int max_bounces = 5;
float depth_min = 0;
float depth_max = 1;
float cutoff_weight = 0.01f;
bool ifShadeBack = false;
bool ifGUI = false;
bool ifShadows = false;
bool ifStats = false;

RayTracer *raytracer;
SceneParser *p;

extern float phiStep, thetaStep;
extern bool isGouraud, ifGrid, ifGridVisual;
extern int nx,ny,nz;
void render();
void traceRay(float a,float b);

int main(int argc,char** argv)
{
    parseCommandArg(argc,argv);
    
    p = new SceneParser(input_file.c_str());

    raytracer = new RayTracer(p,max_bounces,cutoff_weight,ifShadows,ifShadeBack);

    Image *image,*depthImage,*normalImage;

    if (output_file.length() > 0) image = new Image(width,height);
    if (depth_file.length() > 0) depthImage = new Image(width,height);
    if (normal_file.length() > 0) normalImage = new Image(width,height);
    
    // Initialize the raytracing statistics
    RayTracingStats::Initialize(width,height,p->getGroup()->getBoundingBox(),nx,ny,nz);
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            
            Vec2f point;
            int longer = max(width,height); // For the image which width don't equal height.
            if (longer == width) point = {(float)i/width,(float)j/longer + 0.5f * (1 - (float)height / longer)};
            else point = {(float)i/longer + 0.5f * (1 - (float)width / longer),(float)j/height};
            
            Ray ray = p->getCamera()->generateRay(point); //get ray from camera
            float tmin = p->getCamera()->getTMin(); //get tmin of this camera
            
            Hit inter;
            
            Vec3f color = raytracer->traceRay(ray,tmin,0,1,1,inter);
            //if(i == 105 && j == 105)cout<<ray<<endl;
            if(output_file.length() > 0)image->SetPixel(i,j,color);
            
            Vec3f normal = inter.getNormal();
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
    if(ifStats)RayTracingStats::PrintStatistics();

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
        canvas->initialize(p,render,traceRay,raytracer->getGrid(),ifGridVisual);
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
        } else if(!strcmp(argv[i],"-shadows")){
            ifShadows = true;
        } else if(!strcmp(argv[i],"-bounces")){
            i++; assert(i < argc);
            max_bounces = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-weight")){
            i++; assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else if(!strcmp(argv[i],"-grid")){
            ifGrid = true;
            i++; assert(i < argc);
            nx = atoi(argv[i]);
            i++; assert(i < argc);
            ny = atoi(argv[i]);
            i++; assert(i < argc);
            nz = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-visualize_grid")){
            ifGridVisual = true;
        } else if(!strcmp(argv[i],"-stats")){
            ifStats = true;
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

// This func used to generate test ray in GUI
void traceRay(float x,float y){
    Vec2f point = {x,y};
    Ray ray = p->getCamera()->generateRay(point); //get ray from orthographic camera
    float tmin = p->getCamera()->getTMin(); //get tmin of this camera
    
    Hit inter;
    raytracer->traceRay(ray,tmin,0,1,1,inter);
}