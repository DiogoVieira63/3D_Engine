//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_ROTATION_H
#define CLASS4_ROTATION_H


#include <GL/gl.h>
#include "Transform.h"
#include "../Utils/utils.h"

class Rotation : public Transform {
public:
    float angle;
    float time;

    Rotation(float angle, float x, float y, float z,float time) : Transform(x, y, z) {
        this->angle = angle;
        this->time = time;
    }



    void doAction(float t) {
        float newAng = 0;
        if(time != -1) {
            int start = t / time;
            t -= start * time;
            newAng = 360 * (t / time);
        }
        glRotatef(angle + newAng, x, y, z);
    }
};


#endif //CLASS4_ROTATION_H
