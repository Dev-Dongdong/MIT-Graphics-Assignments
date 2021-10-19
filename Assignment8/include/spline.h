/*
 * @Description: Varieties of Spline
 * @Author: LvBujing
 * @Date: 2021-03-09 11:05:25
 * @LastEditTime: 2021-03-11 16:59:47
 * @LastEditors: LvBujing
 */

#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "vectors.h"
#include "triangle_mesh.h"
#include "arg_parser.h"
#include "matrix.h"

//  The Base class of splines
class Spline {

protected:
    int num_vertices;
    Vec3f* vertices;
public:
    Spline(/* args */) { }
    ~Spline() { delete vertices;}

    //  FUNCTIONS
    //  FOR INITIALIZATION
    virtual void set(int &i, Vec3f &v) const { assert(i >=0 && i < num_vertices); vertices[i] = v; }

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) { }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) { }
    virtual void OutputBSpline(FILE *file) { }

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() { return num_vertices; }
    virtual Vec3f getVertex(int i) { assert(i >=0 && i < num_vertices); return vertices[i]; }
    
    
    // FOR EDITING OPERATIONS
    //  Move control points in gui
    virtual void moveControlPoint(int selectedPoint, float x, float y) { 
        assert(selectedPoint >= 0 && selectedPoint < num_vertices);
        Vec3f newPos = getVertex(selectedPoint);
        newPos.Set(x, y, newPos.z());
        set(selectedPoint, newPos);
    }

    //  Naive add control points in gui (Bezier will ovewrite this func)
    virtual void addControlPoint(int selectedPoint, float x, float y) {
        assert(selectedPoint > 0 && selectedPoint < num_vertices);
        num_vertices++;
        Vec3f* new_vertices = new Vec3f[num_vertices];
        for (int i = 0; i < selectedPoint; i++) {
            new_vertices[i] = vertices[i];
        }
        new_vertices[selectedPoint] = Vec3f(x, y, vertices[selectedPoint].z());
        
        for (int i = selectedPoint + 1; i < num_vertices; i++) {
            new_vertices[i] = vertices[i - 1];
        }
        delete [] vertices;
        vertices = new Vec3f[num_vertices];
        copy(new_vertices, new_vertices + num_vertices, vertices);
        delete [] new_vertices;
        
    }

    //  Standard delete control points in gui (Bezier will overwrite this func)
    virtual void deleteControlPoint(int selectedPoint) { 
        if (num_vertices <= 4) return;
        assert(selectedPoint >= 0 && selectedPoint < num_vertices);
        num_vertices--;
        Vec3f* new_vertices = new Vec3f [num_vertices];
        for (int i = 0; i < selectedPoint; i++) {
            new_vertices[i] = vertices[i];
        }
        for (int i = selectedPoint + 1; i < num_vertices + 1; i++) {
            new_vertices[i - 1] = vertices[i];
        }
        delete [] vertices;
        vertices = new Vec3f [num_vertices];
        copy(new_vertices, new_vertices + num_vertices, vertices);
        delete [] new_vertices;
    }

    // FOR GENERATING TRIANGLES
    //  DO NOT USE THIS VERSION
    virtual TriangleMesh* OutputTriangles(ArgParser *args) { assert(0);return NULL; }

};


#endif