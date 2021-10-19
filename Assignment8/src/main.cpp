/*
 * @Description: Main
 * @Author: LvBujing
 * @Date: 2021-03-09 10:51:41
 * @LastEditTime: 2021-03-09 15:51:13
 * @LastEditors: LvBujing
 */
#include <stdio.h>
#include <assert.h>
#include "GL/gl.h"
#include "GL/glut.h"
using namespace std;

#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  // parse the command line arguments & input file
  ArgParser *args = new ArgParser(argc,argv); //  Command line
  SplineParser* splines = new SplineParser(args->input_file); //  Input File

  // launch curve editor!
  if (args->gui) {
    glutInit(&argc,argv);
    GLCanvas glcanvas;
    glcanvas.initialize(args,splines);
    // this never returns...
  }

  // output as required
  splines->SaveBezier(args);
  splines->SaveBSpline(args);
  splines->SaveTriangles(args);

  // cleanup
  delete args;
  delete splines;
  return 0;
}

// ====================================================================
// ====================================================================



