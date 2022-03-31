//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_TRANSLATION_H
#define CLASS4_TRANSLATION_H


#include <GL/gl.h>
#include "Transform.h"

class Translation : public Transform {
public:
    Translation(float x, float y, float z) : Transform(x, y, z) {}

    void doAction() {
        glTranslatef(x, y, z);
    }
};



#endif //CLASS4_TRANSLATION_H
