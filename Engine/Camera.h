//
// Created by diogo on 30/04/22.
//

#ifndef CLASS4_CAMERA_H
#define CLASS4_CAMERA_H


#include <GL/glu.h>

#include <stdio.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include "../Utils/utils.h"



class Camera {
public:

    float pos[3];
    float alpha,beta;
    float d[3];
    float up[3];
    float radiusMax;
    float sensibility = 1;
    float fov,near,far;

    void setPos(float posX,float posY,float posZ);

    void setAlphaBeta(float alpha, float beta);

    void setUp(float upX,float upY,float upZ);

    void setD(float dX,float dY,float dZ);

    void setRadiusMax(float rMax);

    void setCamera();

    void changeDir(float deltaX, float deltaY);


    void moveCamera(float deltaY, float pX, float pY, float pZ);

    void updateDirAngles(int xx, int yy, int startX, int startY);
};


#endif //CLASS4_CAMERA_H
