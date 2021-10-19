/*
 * @Description: Main Execuctable CPP
 * @Author: LvBujing
 * @Date: 2020-12-23 13:14:28
 * @LastEditTime: 2021-09-27 16:43:20
 * @LastEditors: LvBujing
 */
#include <iostream>
#include <cstring>
#include "image.h"
#include "matrix.h"
#include "IFS.h"

using namespace std;
int main(int argc, const char** argv)
{
    string input_file;
    int num_points = 10000;
    int num_iters = 10;
    int size = 100;
    const char *output_file = NULL;

    //cout<<"here is the input_file name:"<<input_file<<endl;
    //命令行解析
    for (int i = 1; i < argc; i++) {    
        if (!strcmp(argv[i],"-input")) {    //获取输入文件
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-points")) {    //获取总点数
            i++; assert (i < argc); 
            num_points = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-iters")) { //获取迭代轮数
            i++; assert (i < argc); 
            num_iters = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-size")) {  //获取大小
            i++; assert (i < argc); 
            size = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {    //获取输出文件
            i++; assert (i < argc); 
            output_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    input_file  = "../" + input_file;
    //cout<<input_file<<endl;
    
    // open the file
    FILE *input = fopen(input_file.c_str(),"r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms; 
    fscanf(input,"%d",&num_transforms);

    IFS ifs(num_transforms);
    
    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability;
        fscanf (input,"%f",&probability);
        Matrix m;
        m.Read3x3(input);
        
        ifs.addMatricesAndProbability(i,m,probability);
    }

    // close the file
    fclose(input);

    Image* image = new Image(size,size);
    image->SetAllPixels(Vec3f(1,1,1));
    //cout<<num_points<<" "<<num_iters<<endl;
    ifs.render(image,num_points,num_iters);
    cout<<"render complete"<<endl;
    
    image->SaveTGA(output_file);
    
    return 0;
}