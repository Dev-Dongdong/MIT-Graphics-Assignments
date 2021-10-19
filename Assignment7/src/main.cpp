/*
 * @Description: Main Executable CPP
 * @Author: LvBujing
 * @Date: 2020-12-23 20:08:02
 * @LastEditTime: 2021-03-06 10:10:43
 * @LastEditors: LvBujing
 */
#include "hit.h"
#include "sampler.h"
#include "film.h"
#include "filter.h"
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
string input_file = "", output_file = "", depth_file = "",normal_file = "",samples_file = "",filter_file = "";
int width = 100;
int height = 100;
int max_bounces = 5;
int zoom_factor = 1;
float depth_min = 0;
float depth_max = 1;
float cutoff_weight = 0.01f;
bool ifShadeBack = false;
bool ifGUI = false;
bool ifShadows = false;
bool ifStats = false;
enum SamplerType { UnSpecified, RandomSamples, UniformSamples, JitteredSamples} samplerType; // Different kinds of Samplers
enum FilterType  { NoFilter, BoxType,     TentType,     GaussianType } filterType;  // Different kinds of Filters
int num_samples = 1;    //  Use to specify the nums of samples of each pixel
float filterParameter = 0.5;    // Use to specify the radius of filter
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
    
    Film *film = new Film(width,height,num_samples);

    Sampler *sampler; 
    Filter *filter;
    switch (samplerType) {  //  Choose different sampler according to COMMAND LINE
        case RandomSamples:     // Randomly sample nums point inside a pixel
            sampler = new RandomSampler(num_samples);
            break;

        case JitteredSamples:   // Unifromly seperate the pixel into subpixels, randomly sample nums point inside each subpixel
            sampler = new JitteredSampler(num_samples);
            break;

        default:    //Unspecified and expecified uniformsampler will use a Uniform Sampler
            sampler = new UniformSampler(num_samples);
            break; 
    }
    switch (filterType) {  //  Choose different filter according to COMMAND LINE
        case BoxType:     
            filter = new BoxFilter(filterParameter);
            break;

        case TentType:   
            filter = new TentFilter(filterParameter);
            break;

        case GaussianType:
            filter = new GaussianFilter(filterParameter);
            break;

        default:    //Unspecified 
            filter = new BoxFilter(filterParameter);
            break; 
    }
    
    // Initialize the raytracing statistics
    RayTracingStats::Initialize(width,height,p->getGroup()->getBoundingBox(),nx,ny,nz);
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            Vec3f color;
            Hit inter;
            for(int s = 0; s < num_samples; s++) {  // Loop all the samples (subpixels)
                Vec2f offset = sampler->getSamplePosition(s);   //  i,j will locate the left bottom of a pixel, while offset moves it.
                Vec2f point;
                float x = i + offset.x() , y = j + offset.y();  //  Here offset is between 0,1
                int longer = max(width,height); // For the image which width don't equal height
                if (longer == width) point = {x/width,y/longer + 0.5f * (1 - (float)height / longer)};
                else point = {x/longer + 0.5f * (1 - (float)width / longer), y/height};
                
                Ray ray = p->getCamera()->generateRay(point); //get ray from camera
                float tmin = p->getCamera()->getTMin(); //get tmin of this camera
                
                Hit freshInter; //  Every loop the inter should clear
                color = raytracer->traceRay(ray,tmin,0,1,1,freshInter);
                inter = freshInter;
                film->setSample(i,j,s,offset,color);    //  Save each sample's color information
            }
            //if(i == 105 && j == 105)cout<<ray<<endl;
            
            
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
    if(output_file.length() > 0){
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                Vec3f color = filter->getColor(i,j,film);   //  Filt the samplers
                image->SetPixel(i,j,color);
            }
        }
    }
    if(ifStats)RayTracingStats::PrintStatistics();  //  Print the stats

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

    if(samples_file.length() > 0){  
        samples_file = "../output_images/" + samples_file;
        film->renderSamples(samples_file.c_str(), zoom_factor);
    }

    if(filter_file.length() > 0){  
        filter_file = "../output_images/" + filter_file;
        film->renderFilter(filter_file.c_str(), zoom_factor, filter);
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
        } else if(!strcmp(argv[i],"-render_samples")){
            i++; assert (i < argc); 
            samples_file = argv[i];
            i++; assert (i < argc);
            zoom_factor = stoi(argv[i]);
        } else if(!strcmp(argv[i],"-render_filter")){
            i++; assert (i < argc); 
            filter_file = argv[i];
            i++; assert (i < argc);
            zoom_factor = stoi(argv[i]);
        } else if(!strcmp(argv[i],"-random_samples")){
            samplerType = RandomSamples;
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-uniform_samples")){
            samplerType = UniformSamples;
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-jittered_samples")){
            samplerType = JitteredSamples;
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
        } else if(!strcmp(argv[i],"-box_filter")){
            filterType = BoxType;
            i++; assert(i < argc);
            filterParameter = stof(argv[i]);
        } else if(!strcmp(argv[i],"-tent_filter")){
            filterType = TentType;
            i++; assert(i < argc);
            filterParameter = stof(argv[i]);
        } else if(!strcmp(argv[i],"-gaussian_filter")){
            filterType = GaussianType;
            i++; assert(i < argc);
            filterParameter = stof(argv[i]);
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