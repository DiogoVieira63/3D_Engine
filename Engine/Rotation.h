//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_ROTATION_H
#define CLASS4_ROTATION_H


#include <GL/gl.h>
#include "Transform.h"

class Rotation : public Transform {
public:
    float angle;

    Rotation(float angle, float x, float y, float z) : Transform(x, y, z) {
        this->angle = angle;
    }

    void doAction() {
        glRotatef(angle, x, y, z);
    }
};


#endif //CLASS4_ROTATION_H
