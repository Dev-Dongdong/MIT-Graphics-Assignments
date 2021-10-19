/*
 * @Description: A little change
 * @Author: LvBujing
 * @Date: 2021-03-11 18:15:49
 * @LastEditTime: 2021-03-13 10:58:23
 * @LastEditors: LvBujing
 */
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include "GL/gl.h"
#include "GL/glut.h"
using namespace std;

#include "glCanvas.h"
#include "parser.h"

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  // command line argument defaults
  const char *filename = NULL;
  float refresh = 0.1;  // in seconds
  float dt = 0.1;       // in seconds
  int integrator_color = 0;
  int draw_vectors = 0;
  float acceleration_scale = 1;
  int motion_blur = 0;
 
  // parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i],"-input")) {
      i++; assert (i < argc);      
      filename = argv[i];
    } else if (!strcmp(argv[i],"-refresh")) {
      i++; assert (i < argc);      
      refresh = atof(argv[i]);
    } else if (!strcmp(argv[i],"-dt")) {
      i++; assert (i < argc);      
      dt = atof(argv[i]);
    } else if (!strcmp(argv[i],"-integrator_color")) {
      integrator_color = 1;
    } else if (!strcmp(argv[i],"-motion_blur")) {
      motion_blur = 1;
    } else if (!strcmp(argv[i],"-draw_vectors")) {
      draw_vectors = 1;
      i++; assert (i < argc);      
      acceleration_scale = atof(argv[i]);
    } else {
      printf ("WARNING:  unknown command line argument %s\n", argv[i]);
      assert(0);
    }
  }

  // load the particle system
  assert (filename != NULL);
  string input_path = filename;
  input_path = "../input_files/" + input_path;
  Parser *parser = new Parser(input_path.c_str());
  
  // launch viewer!   and it never returns...
  GLCanvas glcanvas;
  glutInit(&argc,argv);
  glcanvas.initialize(parser,refresh,dt,integrator_color,
		      draw_vectors,acceleration_scale,motion_blur);
  return 0;
}

// ====================================================================
// ====================================================================



