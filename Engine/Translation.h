//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_TRANSLATION_H
#define CLASS4_TRANSLATION_H


#include <GL/gl.h>
#include "Transform.h"
#include <utility>
#include <vector>
#include <cmath>
#include "../Utils/utils.h"

using namespace std;

class Translation : public Transform {
public:
    vector<Ponto> points;
    Ponto pointY = Ponto(0, 1, 0);
    float time;
    bool align;
    vector<Ponto>curvePoints;


    Translation(float x, float y, float z,vector<Ponto> vector,float num, bool align) : Transform(x, y, z) {
        this->points = vector;
        this->time = num;
        this->align = align;
        if (!vector.empty())setCurvePoints();
    }
    void setCurvePoints(){
        float pos[4];
        float deriv[4];
        int parts = 100;
        float part = 1.0f/(float )parts;
        for (int i = 0; i <parts;i++) {
            getGlobalCatmullRomPoint(part*(float)i, pos, deriv);
            Ponto p(pos[0], pos[1], pos[2]);
            curvePoints.push_back(p);
        }
    }

    void getCatmullRomPoint(float t, Ponto p0,Ponto p1, Ponto p2,Ponto p3, float *pos, float *deriv) {

        // catmull-rom matrix
        float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                             { 1.0f, -2.5f,  2.0f, -0.5f},
                             {-0.5f,  0.0f,  0.5f,  0.0f},
                             { 0.0f,  1.0f,  0.0f,  0.0f}};

        for (int i = 0; i < 3;i++){
            float vector[4] = {p0.get(i),p1.get(i),p2.get(i),p3.get(i)};
            float a[4];
            utils::multMatrixVector(m, vector, a);
            pos[i] = powf(t,3) * a[0] + powf(t,2) * a[1] + t * a[2] + a[3];
            deriv[i]= 3 * powf(t,2) * a[0]+ 2* t * a[1] + a[2];
        }
    }

// given  global t, returns the point in the curve
    void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {
        int size = points.size();

        float t = gt * (float) size; // this is the real global t
        int index = floor(t);  // which segment
        t = t - index; // where within  the segment

        // indices store the points
        int indices[4];
        indices[0] = (index + size-1)%size;
        indices[1] = (indices[0]+1)%size;
        indices[2] = (indices[1]+1)%size;
        indices[3] = (indices[2]+1)%size;

        getCatmullRomPoint(t, points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]], pos, deriv);
    }


    void renderCatmullRomCurve() {
        glBegin(GL_LINE_LOOP);

        for (Ponto p : curvePoints)
            glVertex3f(p.x,p.y,p.z);

        glEnd();

    }


    void alignModel(float *deriv){
        float x[3] = {deriv[0], deriv[1], deriv[2]};
        utils::normalize(x);

        float ant_y[3] = {pointY.x, pointY.y, pointY.z};
        float z[3];
        utils::cross(x, ant_y, z);
        utils::normalize(z);

        float y[3];
        utils::cross(z, x, y);
        utils::normalize(y);

        pointY.x = y[0];
        pointY.y = y[1];
        pointY.z = y[2];

        float m[16];
        utils::buildRotMatrix(x, y, z, m);
        glMultMatrixf(m);
    }

    void doAction(float t) {
        if(!points.empty()){
            renderCatmullRomCurve();
            int start = t/time;
            t -= start*time;
            float pos[4],deriv[4];
            getGlobalCatmullRomPoint(t/time,pos,deriv);
            glTranslatef(pos[0],pos[1],pos[2]);

            if (align) alignModel(deriv);
        }
        else
            glTranslatef(x, y, z);
    }
};



#endif //CLASS4_TRANSLATION_H
