/*
 * @Description: Two types of Curves
 * @Author: LvBujing
 * @Date: 2021-03-09 12:45:22
 * @LastEditTime: 2021-10-03 21:15:03
 * @LastEditors: LvBujing
 */
#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "GL/gl.h"
#include "GL/glut.h"

//  A type of Spline, and the base class of Curves
class Curve : public Spline {

public:
    Curve(int _num_vertices) { num_vertices = _num_vertices; vertices = new Vec3f[num_vertices]; }
    ~Curve() { }

    virtual int getInterval () = 0;
    virtual Vec3f getVertexInCurve (const int &index, float t) const = 0;
    virtual void Paint(ArgParser *args) { 
        //  Render bound lines(Blue)
        glColor3f(0.0, 0.0, 1.0);
        glLineWidth(1);
        glBegin(GL_LINES);
        for (int i = 1; i < num_vertices; i++) {
            glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
            glVertex3f(vertices[i-1].x(), vertices[i-1].y(), vertices[i-1].z());
        }  
        glEnd();
    }
    void drawControlPoints() {
        // draw control points(Red)
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(5);
        glBegin(GL_POINTS);
        for (int i = 0; i < num_vertices; i++) {
            glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
        }    
        glEnd();
    }

    //  Directly Output Current controll points as Bezier
    virtual void OutputBezier(FILE *file) { 
        fprintf (file, "\nbezier\n");
        fprintf (file, "num_vertices %d\n", getNumVertices());
        for (int i = 0; i < num_vertices; i++) {
            fprintf (file, "%f %f %f\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }

    //  Directly Output Current controll points as BSpline
    virtual void OutputBSpline(FILE *file) { 
        fprintf (file, "\nbspline\n");
        fprintf (file, "num_vertices %d\n", getNumVertices());
        for (int i = 0; i < num_vertices; i++) {
            fprintf (file, "%f %f %f\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }

    //  Helper for Bezier->BSpline & BSpline->Bezier
    //  Move the spline basis matrix from another side of equation to this side
    //  Then calculate the changed control point and save it to file
    //  TODO: Find a way to handle Change between Bezier & BSpline while control points' num is > 4
    void matrixManipulate(Matrix &B1, Matrix &B2, FILE *file, int interval) const {
        Matrix Gb1,Bb2 = B2;
        Bb2.Inverse();
        int index = 0;
        while (index + interval < num_vertices) {
            for (int i = index; i < index + 4; i++) {
                Gb1.Set(i - index,0,vertices[i].x());
                Gb1.Set(i - index,1,vertices[i].y());
                Gb1.Set(i - index,2,vertices[i].z());
                Gb1.Set(i - index,3,0);
            }
            Gb1 *= B1 * Bb2;
            for (int i = 0; i < 4; i++) {
                fprintf (file, "%f %f %f\n", Gb1.Get(i,0), Gb1.Get(i,1), Gb1.Get(i,2));
            }
            index += interval;
        }
    }
};


//  A type of Curve, controlled by points, pass through the begin and end points.
class BezierCurve : public Curve {

private:
    //  Helper func for 4 steps bezier
    float BezierCoefficient(float p1, float p2, float p3, float p4, float t) const {
        float m = 1 - t;
        return m*m*m*p1 + 3*t*m*m*p2 + 3*t*t*m*p3 + t*t*t*p4;
    }
    
public:
    BezierCurve(int _num_vertices) : Curve(_num_vertices) { }
    BezierCurve () : Curve(4) { }
    ~BezierCurve() { }
    
    //  Get the interval between neighbor 4 points bezier
    int getInterval () { return 3;}

    //  Give a control points index (the begin one of every 4 points) and t, return a point in curve
    Vec3f getVertexInCurve (const int &index, float t) const {
        float x,y,z;
        x = BezierCoefficient(vertices[index].x(), vertices[index+1].x(), vertices[index+2].x(), vertices[index+3].x(), t);
        y = BezierCoefficient(vertices[index].y(), vertices[index+1].y(), vertices[index+2].y(), vertices[index+3].y(), t);
        z = BezierCoefficient(vertices[index].z(), vertices[index+1].z(), vertices[index+2].z(), vertices[index+3].z(), t);
        return Vec3f(x,y,z);
    }

    //  Paint with OpenGL gui
    void Paint(ArgParser *args) {
        int samples = args->curve_tessellation;   //  Samples between two points
        Curve::Paint(args);     //  Render the controll points and bound lines
        
        //  Render Bezier curve
        glColor3f(0.0, 1.0, 0.0);
        glLineWidth(3);
        glBegin(GL_LINES);
        float x,y,z;
        int index;
        for (int n = 1; n <= (num_vertices - 1) / 3; n++) {
            index = (n - 1) * 3;
            for (int i = 0; i <= samples; i++) {
                x = BezierCoefficient(vertices[index].x(), vertices[index + 1].x(), vertices[index + 2].x(), vertices[index + 3].x(), (float)i / samples);
                y = BezierCoefficient(vertices[index].y(), vertices[index + 1].y(), vertices[index + 2].y(), vertices[index + 3].y(), (float)i / samples);
                z = BezierCoefficient(vertices[index].z(), vertices[index + 1].z(), vertices[index + 2].z(), vertices[index + 3].z(), (float)i / samples);
                glVertex3f(x, y, z);
                if(i != 0 && i != samples) glVertex3f(x, y, z);
            }
        }
        glEnd();
        drawControlPoints();
    }

    void OutputBezier(FILE *file) { Curve::OutputBezier(file); } //  The spline is already bezier
    
    void OutputBSpline(FILE *file) {    //  Need Matrix calculate
        fprintf (file, "\nbspline\n");
        fprintf (file, "num_vertices %d\n", getNumVertices());
        float *p = new float[16] {-1,3,-3,1, 3,-6,3,0, -3,3,0,0, 1,0,0,0};
        float *q = new float[16] {-1,3,-3,1, 3,-6,0,4, -3,3,3,1, 1,0,0,0};
        Matrix Bbe(p),Bbs(q);
        Bbs *= (float)1 / 6;
        matrixManipulate(Bbe,Bbs,file,3); //  Move the BSpline basis matrix to Bezier side 
        delete p;
        delete q;
     }

     //  Bezier add control points in gui
    void addControlPoint(int selectedPoint, float x, float y) {
        Spline::addControlPoint(selectedPoint, x, y);
        Vec3f pre = getVertex(selectedPoint - 1);
        Vec3f nxt = getVertex(selectedPoint + 1);
        float pre_x,pre_y,nxt_x,nxt_y;
        nxt_x = (x + nxt.x()) / 2; nxt_y = (y + nxt.y()) / 2;
        pre_x = (x + pre.x()) / 2; pre_y = (y + pre.y()) / 2;
        Spline::addControlPoint(selectedPoint + 1, nxt_x, nxt_y);
        Spline::addControlPoint(selectedPoint    , pre_x, pre_y);
    }
    
    //  Bezier delete control points in gui
    void deleteControlPoint(int selectedPoint) { 
        if (num_vertices <= 4) return;
        Spline::deleteControlPoint(selectedPoint);
        while ((num_vertices - 1) % 3 != 0) {
            if (selectedPoint >= num_vertices) selectedPoint = num_vertices - 1; 
            Spline::deleteControlPoint(selectedPoint);
        }
    }
};

//  A type of Curve, controlled by points, not pass through any controll points
class BSplineCurve : public Curve {

private:
    //  Help Func
    float BSplineCoefficient(float p1, float p2, float p3, float p4, float t) const {
        float m = 1 - t;
        float t_2 = t*t;
        float t_3 = t_2 * t;
        return (m*m*m*p1 + (3*t_3 - 6*t_2 + 4)*p2 + (-3*t_3 + 3*t_2 + 3*t + 1)*p3 + t_3*p4) / 6;
    }
public:
    BSplineCurve(int _num_vertices) : Curve(_num_vertices) { }
    ~BSplineCurve();
    //  Get the interval between neighbor 4 points BSpline
    int getInterval () { return 1; }

    //  Give a control points index (the begin one of every 4 points) and t, return a point in curve
    Vec3f getVertexInCurve (const int &index, float t) const {
        float x,y,z;
        x = BSplineCoefficient(vertices[index].x(), vertices[index+1].x(), vertices[index+2].x(), vertices[index+3].x(), t);
        y = BSplineCoefficient(vertices[index].y(), vertices[index+1].y(), vertices[index+2].y(), vertices[index+3].y(), t);
        z = BSplineCoefficient(vertices[index].z(), vertices[index+1].z(), vertices[index+2].z(), vertices[index+3].z(), t);
        return Vec3f(x,y,z);
    }

    //  Paint with OpenGL gui
    void Paint(ArgParser *args) {
        int samples = args->curve_tessellation;   //  Samples between two points
        samples *= 10;
        Curve::Paint(args);     //  Render the controll points and bound lines
    
        //  Render BSpline curve
        glColor3f(0.0, 1.0, 0.0);
        glPointSize(3);
        glBegin(GL_POINTS);
        float x,y,z;
        int index;
        for (int n = 1; n <= num_vertices - 3; n++) {
            index = (n - 1);
            for (int i = 0; i < samples; i++) {
                x = BSplineCoefficient(vertices[index].x(), vertices[index + 1].x(), vertices[index + 2].x(), vertices[index + 3].x(), (float)i / samples);
                y = BSplineCoefficient(vertices[index].y(), vertices[index + 1].y(), vertices[index + 2].y(), vertices[index + 3].y(), (float)i / samples);
                z = BSplineCoefficient(vertices[index].z(), vertices[index + 1].z(), vertices[index + 2].z(), vertices[index + 3].z(), (float)i / samples);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
        drawControlPoints();
    }
    
    void OutputBSpline(FILE *file) { Curve::OutputBSpline(file); }  //  The spline is already BSpline

    void OutputBezier(FILE *file) {    //  Need Matrix calculate
        fprintf (file, "\nbezier\n");
        fprintf (file, "num_vertices %d\n", getNumVertices());
        float *p = new float[16] {-1,3,-3,1, 3,-6,3,0, -3,3,0,0, 1,0,0,0};
        float *q = new float[16] {-1,3,-3,1, 3,-6,0,4, -3,3,3,1, 1,0,0,0};
        Matrix Bbe(p),Bbs(q);
        Bbs *= (float)1 / 6;
        matrixManipulate(Bbs,Bbe,file,3);  //  Move the Bezier basis matrix to BSpline side 
        
        delete p;
        delete q;
     }
};

#endif