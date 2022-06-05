//
// Created by diogo on 02/06/22.
//

#ifndef CLASS4_LIGHT_H
#define CLASS4_LIGHT_H


#include "Ponto.h"
#include <GL/gl.h>
class Light {

public:

    Ponto position;
    int lightNumber;

    Light(Ponto pos,int num){
        this->position = pos;
        this->lightNumber = GL_LIGHT0 + num;
    }

    virtual void doAction() = 0;
};

class LightPoint : public Light{
public:
    LightPoint(Ponto pos, int num) : Light(pos, num) {
    }

    void doAction(){
        float pos[4] = {position.x, position.y, position.z,1};
        glLightfv(lightNumber, GL_POSITION, pos);
    }

};


class LightDirectional : public Light{
public:

    LightDirectional(Ponto pos, int num) : Light(pos, num) {
    }

    void doAction(){
        float pos[4] = {position.x, position.y, position.z,0};
        glLightfv(lightNumber, GL_POSITION, pos);
    }

};

class LightSpotLight : public Light{
public:
    Ponto dir;
    float cuttof;

    LightSpotLight(Ponto pos, int num, Ponto dir, float value) : Light(pos, num) {
        this->dir = dir;
        this->cuttof = value;
    }

    void doAction(){
        float pos[4] = {position.x, position.y, position.z,1};
        glLightfv(lightNumber,GL_POSITION, pos);
        float direction[3] = {dir.x, dir.y, dir.z};
        glLightfv(lightNumber,GL_SPOT_DIRECTION, direction);
        glLightf(lightNumber,GL_SPOT_CUTOFF,cuttof);
    }

};

#endif //CLASS4_LIGHT_H
