/*
 * @Description: Main Executable CPP
 * @Author: LvBujing
 * @Date: 2020-12-23 20:08:02
 * @LastEditTime: 2020-12-25 12:54:44
 * @LastEditors: LvBujing
 */
#include "hit.h"
#include "material.h"
#include "cstring"
//#include "ray.h"
#include "image.h"
#include "Group.h"
#include "Camera.h"
#include "scene_parser.h"


using namespace std;

#define EPSILON 0.0001f
int main(int argc,char** argv)
{
    string input_file = "";
    int width = 100;
    int height = 100;
    string output_file = "";
    float depth_min = 0;
    float depth_max = 1;
    string depth_file = "";

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
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    input_file = "../" + input_file;
    SceneParser *p = new SceneParser(input_file.c_str());
    Image *image = new Image(width,height);
    Image *depthImage = new Image(width,height);

    // set background color
    image->SetAllPixels(p->getBackgroundColor());

    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            
            Vec2f point = {(float)i/width,(float)j/height};
            Ray ray = p->getCamera()->generateRay(point); //get ray from orthographic camera
            float tmin = p->getCamera()->getTMin(); //get tmin of this camera
            Hit inter;
            if(p->getGroup()->intersect(ray,inter,tmin)){   //if this ray hit a object, the intersection information will be stored at 'inter' 
                Vec3f color;
            
                // obtain the color for this pixel
                assert(inter.getMaterial() != NULL);
                color = inter.getMaterial()->getDiffuseColor();
                
                image->SetPixel(i,j,color);
                
                // tackle the depth image
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
    output_file = "../" + output_file;
    depth_file = "../" + depth_file;
    image->SaveTGA(output_file.c_str());
    depthImage->SaveTGA(depth_file.c_str());

    delete p;
    delete image;
    delete depthImage;
    return 0;
}
