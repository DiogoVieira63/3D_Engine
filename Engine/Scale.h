//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_SCALE_H
#define CLASS4_SCALE_H


#include <GL/gl.h>
#include "Transform.h"

class Scale : public Transform {
public:
    Scale(float x, float y, float z) : Transform(x, y, z) {}

    void doAction(float t) {
        glScalef(x, y, z);
    }
};

#endif //CLASS4_SCALE_H
